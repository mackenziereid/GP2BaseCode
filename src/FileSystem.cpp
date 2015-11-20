#include "FileSystem.h"

#ifdef _WIN32
#include <Windows.h>
#endif

void ChangeWorkingDirectory()
{
#ifdef _WIN32
    char buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, MAX_PATH);
    string exeFullFileName(buffer);
    string exeDirectory = exeFullFileName.substr(0, exeFullFileName.find_last_of("\\"));
    SetCurrentDirectory(exeDirectory.c_str());
#endif
}