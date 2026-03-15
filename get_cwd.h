#ifndef GET_CWD_HEADER
#define GET_CWD_HEADER

#define cwd_max_len 1024

#ifdef _WIN32
#include <direct.h>
#define get_cwd _getcwd
#else
#include <unistd.h>
#define get_cwd getcwd
#endif

#define pastecwd(destination) get_cwd(destination, sizeof(destination))

#endif