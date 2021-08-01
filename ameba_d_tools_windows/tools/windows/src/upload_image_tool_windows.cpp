/*

Compile:
windows:
mingw32-g++.exe -o upload_image_tool_windows.exe upload_image_tool_windows.cpp -static
i686-w64-mingw32-g++.exe -o upload_image_tool_windows.exe upload_image_tool_windows.cpp -static

linux:
g++ -m32 -std=c++11 -lpthread -static -o upload_image_tool_linux upload_image_tool_linux.cpp
g++ -std=c++11 -lpthread -static -o upload_image_tool_linux upload_image_tool_linux.cpp

macos:
g++ -std=c++11 -lpthread -o upload_image_tool_macos upload_image_tool_macos.cpp

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

#include <thread>
//#include <mutex>

#include <time.h>
#include <errno.h>  

using namespace std;



#if defined(__WIN32__) // MINGW64
#include <windows.h>
#define FD HANDLE
#define sleep_ms(s) msleep(s)
//#define sleep(n) Sleep(n)

#elif defined(__linux__) || defined(__APPLE__)// ubuntu 32 bits  and OS X 64bits
//#error stop.....
#include <sys/ioctl.h>
#include <fcntl.h>
#define FD int
#define sleep_ms(s) msleep(s) //usleep((s*1000))

#else

#error compiler env doesn't supported!

#endif

bool auto_flash = true;

//mutex mu;
int check_image_upload = 0;

int msleep(long msec) {
    struct timespec ts;
    int res;

    if (msec < 0) {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}



int set_DTR_RTS(FD fd, unsigned short level)
{
#if defined(__WIN32__)
/*
	if (level & 0x1) 
        EscapeCommFunction(fd, SETRTS);
	else 
        EscapeCommFunction(fd, CLRRTS);


	if (level & 0x2) 
        EscapeCommFunction(fd, SETDTR);
	else 
        EscapeCommFunction(fd, CLRDTR);
*/
    if(level==0) {
        EscapeCommFunction(fd, CLRRTS);
        EscapeCommFunction(fd, CLRDTR);
    } else if(level==1) {
        EscapeCommFunction(fd, SETRTS);
        EscapeCommFunction(fd, CLRDTR);
    } else if(level==2) {
        EscapeCommFunction(fd, CLRRTS);
        EscapeCommFunction(fd, SETDTR);
    } else {
        EscapeCommFunction(fd, SETRTS);
        EscapeCommFunction(fd, SETDTR);
    }
#elif defined(__linux__) || defined(__APPLE__)

  int status;
  
  ioctl(fd, TIOCMGET, &status);

  if(level & 0x1)
    status |= TIOCM_RTS;
  else
    status &= TIOCM_RTS;

  if(level & 0x2)
    status |= TIOCM_DTR;
  else
    status &= TIOCM_DTR;

  ioctl(fd, TIOCMSET, &status);

#endif
    return 0;
}

void reset_method(char * com , int method) {

    char buf[256];
    FD fd;

#if defined(__WIN32__)

    strcpy(buf,"\\\\.\\");
    strcat(buf,com);

    fd = CreateFileA(buf,                //port name
                        GENERIC_READ | GENERIC_WRITE, //Read/Write
                        0,                            // No Sharing
                        NULL,                         // No Security
                        OPEN_EXISTING,// Open existing port only
                        0,            // Non Overlapped I/O
                        NULL);        // Null for Comm Devices


    if (fd == INVALID_HANDLE_VALUE) {
        printf("Error in opening serial port");
        return;
    }
#elif defined(__linux__) || defined(__APPLE__)

    fd = open( com , O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd <0) {
       printf("Error in opening serial port %s.\n",com);
    }

#endif
    // DTR RTS EN BUN
    //   1  1   1  1
    //   0  0   1  1
    //   1  0   0  1
    //   0  1   1  0

    if(method) {
    
        set_DTR_RTS(fd,0x0); // normal
        sleep_ms(200);
        // enter uart flash download mode.
        set_DTR_RTS(fd,0x1);
        sleep_ms(200);
        set_DTR_RTS(fd,0x2);
        sleep_ms(200);
        set_DTR_RTS(fd,0x0); // normal
        sleep_ms(500);
    } else {
        // reset system.
        set_DTR_RTS(fd,0x0); // normal
        sleep_ms(200);
        set_DTR_RTS(fd,0x2);
        sleep_ms(200);
        set_DTR_RTS(fd,0x3); // normal
        sleep_ms(500);
    }
#if defined(__WIN32__)
    CloseHandle(fd); //Closing the Serial Port
#elif defined(__linux__) || defined(__APPLE__)
    close(fd);
#endif

}

void generate_reset_to_flash( char * com) {
    reset_method(com,1);
}
void generate_reset_to_boot( char * com) {
    reset_method(com,0);
}

void image_tool_thread(char* t1_com) {
    stringstream t1_cmdss;
    string t1_cmd;

    t1_cmdss.clear();

    // run image tool with serial port as argument
#if defined(__WIN32__)
	t1_cmdss << ".\\tools\\windows\\image_tool\\amebad_image_tool.exe " << t1_com;
	getline(t1_cmdss, t1_cmd);
#elif defined (__linux__)
    t1_cmdss << "./tools/linux/image_tool/amebad_image_tool " << t1_com;

    // copy command in cmdss to cmd
    t1_cmd = t1_cmdss.str();
#elif defined(__APPLE__)
    t1_cmdss << "./tools/macos/image_tool/amebad_image_tool " << t1_com;
    getline(t1_cmdss, t1_cmd);
    cout << t1_cmd << endl;
#endif
    system(t1_cmd.c_str());

//  mu.lock();
    check_image_upload = 100;
//  mu.unlock();
}

void image_tool_progress() {

    cout << "Uploading." << flush;
    while (1) {
        sleep_ms(500);
//      mu.lock();
        if (check_image_upload != 0) {
            break;
        } else {
            cout << "." << flush;
        }
//      mu.unlock();
    }
    cout << "    Upload Image done. " << endl;
}

int main(int argc, char *argv[]) {
    string cmd;

    // change directory to {runtime.tools.ameba_d_tools.path}
    chdir(argv[1]);


    if(!auto_flash) {
      // 5 seconds count down to allow user setting ameba D to UART download mode
      cmd = "Please enter the upload mode (wait 5s)";
      cout << cmd << endl;
      for (int i = 5; i > 0; i--) {
        sleep(1);
        //sleep_ms(1000);
        cmd = to_string(i);
        cout << "    0" << cmd << endl;
      }
    }

    if(auto_flash) {
      printf("Enter uart flash download mode..\n");
      generate_reset_to_flash(argv[2]);
    }

    thread t2(image_tool_progress);
    thread t1(image_tool_thread, argv[2]);

//  t2.detach();
//  t1.detach();
    t2.join();
    t1.join();

    if(auto_flash) {
      printf("Hard resetting via RTS pin..\n");
      generate_reset_to_boot(argv[2]);
    }

    return 0;
}
