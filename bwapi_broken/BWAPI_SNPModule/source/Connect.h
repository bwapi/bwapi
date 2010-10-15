#pragma once
#include <windows.h>
#include <Winsock.h>

// use SO_REUSEADDR

#define BASE_PORT 6110

#define i(x) MessageBox(NULL, x, "*", MB_OK | MB_ICONASTERISK)
#define w(x) MessageBox(NULL, x, "!", MB_OK | MB_ICONWARNING)
#define e(x) MessageBox(NULL, x, "Error!", MB_OK | MB_ICONERROR)
