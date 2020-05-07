#include <algorithm>
#include <iostream>

#include <Util/SecureSystem.h>

// Guidance From:
// https://wiki.sei.cmu.edu/confluence/pages/viewpage.action?pageId=87152177
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
//Windows Implementation
#include <Windows.h>

void secsystem(std::string cmd, std::vector<std::string> args) {
    std::string argstr = cmd + " ";
    std::for_each(args.begin(), args.end(), [&argstr](std::string arg) { argstr += arg + " "; });

    STARTUPINFO si = { 0 };
    PROCESS_INFORMATION pi;
    si.cb = sizeof(si);

    LPCSTR command = cmd.c_str();
    LPSTR argline = argstr.data();

    std::cout << argline << std::endl;

    if (!CreateProcess(command, argline, NULL, NULL, FALSE, 0, 0, 0, &si, &pi)) {}
    
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
}
#else
// Posix implementation
//TODO: Make POSIX implementation
#endif