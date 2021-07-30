/*

Compile:

windows:
mingw32-g++.exe -o upload_image_tool_windows.exe upload_image_tool_windows.cpp -static
i686-w64-mingw32-g++.exe -o upload_image_tool_windows.exe upload_image_tool_windows.cpp -static
*/

#include <iostream>
#include <sstream>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <string>
#include <cstdio>
#include <sys/stat.h>
#include <fstream>
#include <unistd.h>

#include <windows.h>

#include <thread>
//#include <mutex>



using namespace std;

#define sleep(n) Sleep(n)


#if defined(__WIN32__)
#define FD HANDLE
#elif defined(__LINUX__)
#define FD int
#elif defined(__APPLE__)
    //<TODO>
#endif

int set_DTR_RTS(FD hComm, unsigned short level)
{
#if defined(__WIN32__)
/*
	if (level & 0x1) 
        EscapeCommFunction(hComm, SETRTS);
	else 
        EscapeCommFunction(hComm, CLRRTS);


	if (level & 0x2) 
        EscapeCommFunction(hComm, SETDTR);
	else 
        EscapeCommFunction(hComm, CLRDTR);
*/
    if(level==0) {
        EscapeCommFunction(hComm, CLRRTS);
        EscapeCommFunction(hComm, CLRDTR);
    } else if(level==1) {
        EscapeCommFunction(hComm, SETRTS);
        EscapeCommFunction(hComm, CLRDTR);
    } else if(level==2) {
        EscapeCommFunction(hComm, CLRRTS);
        EscapeCommFunction(hComm, SETDTR);
    } else {
        EscapeCommFunction(hComm, SETRTS);
        EscapeCommFunction(hComm, SETDTR);
    }
#elif defined(__LINUX__)
    //<TODO>
#elif defined(__APPLE__)
    //<TODO>
#endif
    return 0;
}

void reset_method(char * com , int method) {

    char buf[256];

#if defined(__WIN32__)

    HANDLE hComm;

    strcpy(buf,"\\\\.\\");
    strcat(buf,com);

    hComm = CreateFileA(buf,                //port name
                        GENERIC_READ | GENERIC_WRITE, //Read/Write
                        0,                            // No Sharing
                        NULL,                         // No Security
                        OPEN_EXISTING,// Open existing port only
                        0,            // Non Overlapped I/O
                        NULL);        // Null for Comm Devices


    if (hComm == INVALID_HANDLE_VALUE) {
        printf("Error in opening serial port");
        return;
    }
#elif defined(__LINUX__)
    //<TODO>
#elif defined(__APPLE__)
    //<TODO>
#endif
    // DTR RTS EN BUN
    //   1  1   1  1
    //   0  0   1  1
    //   1  0   0  1
    //   0  1   1  0

    if(method) {
        set_DTR_RTS(hComm,0x0); // normal
        sleep(100);
        // enter uart flash download mode.
        set_DTR_RTS(hComm,0x1);
        sleep(100);
        set_DTR_RTS(hComm,0x2);
        sleep(100);
        set_DTR_RTS(hComm,0x0); // normal
        sleep(500);
    } else {
        // reset system.
        set_DTR_RTS(hComm,0x2);
        sleep(200);
        set_DTR_RTS(hComm,0x3); // normal
        sleep(500);
    }
#if defined(__WIN32__)
    CloseHandle(hComm); //Closing the Serial Port
#elif defined(__LINUX__)
    //<TODO>
#elif defined(__APPLE__)
    //<TODO>
#endif

}

void generate_reset_to_flash( char * com) {
    reset_method(com,1);
}
void generate_reset_to_boot( char * com) {
    reset_method(com,0);
}

//mutex mu;
int check_image_upload = 0;

void image_tool_thread(char* t1_com) {
	stringstream t1_cmdss;
	string t1_cmd;
	t1_cmdss.clear();
	t1_cmdss << ".\\tools\\windows\\image_tool\\amebad_image_tool.exe " << t1_com;
	getline(t1_cmdss, t1_cmd);
	system(t1_cmd.c_str());
//	mu.lock();
	check_image_upload = 100;
//	mu.unlock();
}

void image_tool_progress() {
	cout << "Uploading." << flush;
	while (1) {
		Sleep(500);
//		mu.lock();
		if (check_image_upload != 0) {
			break;
		} else {
			cout << "." << flush;
		}
//		mu.unlock();
	}
	cout << "    Upload Image done. " << endl;
}


int main(int argc, char *argv[]) {
	string cmd;
	chdir(argv[1]);

/*
	cmd = "Please enter the upload mode (wait 5s)";
	cout << cmd << endl;
	for (int i = 5; i > 0; i--)
	{
		Sleep(1000);
		cmd = to_string(i);
		cout << "    0" << cmd << endl;
	}
*/

    printf("Enter uart flash download mode..\n");
    generate_reset_to_flash(argv[2]);

	thread t2(image_tool_progress);
	thread t1(image_tool_thread, argv[2]);

//	t2.detach();
//	t1.detach();
	t2.join();
	t1.join();

    printf("Hard resetting via RTS pin..\n");
    generate_reset_to_boot(argv[2]);

	return 0;
}
