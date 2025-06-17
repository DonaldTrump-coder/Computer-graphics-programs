// GLOS.H
//////////////////////////////////////////////////////////
// This is an OS specific header file
//判别操作系统的基本头文件


#include <windows.h>


// disable data conversion warnings


#pragma warning(disable : 4244) // MIPS
#pragma warning(disable : 4136) // X86
#pragma warning(disable : 4051) // ALPHA