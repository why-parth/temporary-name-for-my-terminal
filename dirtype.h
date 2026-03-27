#ifndef DIRECTORY_TYPE_HEADER
#define DIRECTORY_TYPE_HEADER


/* OS Independent Pre-Processing */


#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>

#include "macro_pastecwd(destination).h"

/* OS Dependent Pre-Processing */

#ifdef _WIN32
// for Windows

#include <windows.h>
#define file_datatype WIN32_FIND_DATAW
#define filename_datatype WCHAR *
#define filename_char_datatype WCHAR
#define filetype_datatype DWORD
#define filetype .dwFileAttributes
#define filename .cFileName
#define is_directory & FILE_ATTRIBUTE_DIRECTORY

#else
// for other than Windows

#include <dirent.h>
#define file_datatype struct dirent *
#define filename_datatype char *
#define filename_char_datatype char
#define filetype_datatype unsigned char
#define filetype ->d_type
#define filename ->d_name
#define is_directory == DT_DIR

#endif

/* Interface Definitions */
#define dl_file_name_length 32

/* Structures */

typedef struct dl_file * fileptr;
typedef struct dl dl;
typedef char dirtype_t;


// fileptr : used for storing files' name, type and the pointer-to next file (thus, it can be used to form a linked list)
struct dl_file {
    filename_char_datatype name[dl_file_name_length + 1];
    filetype_datatype __filetype;
    struct dl_file * next;
    dl * dl;
    dirtype_t type;
};

// dl : used for storing and managing directories 
struct dl {
    char path[cwd_max_len + 1];
    fileptr files;
    unsigned int n_files;
};


// START SEGMENT : UI
// this segment is only for the UI, not related to any logic.

#define dirtype_terminal    0
#define dirtype_explicit    1
#define dirtype_implicit    2
#define dirtype_mainfile    3
#define dirtype_dlparsed    4
#define dirtype_unparsed    5
#define dirtype_document    6


#define DirType __DirType_identifiers + __DirType_identifier_stride * 
#define __DirType_identifier_stride 20
extern char __DirType_identifiers[(__DirType_identifier_stride + 1) * 7];

#define __printDirectoryType(n) printf(DirType n)
#define __printFullDirectoryType(n) printf("%s%s", DirType n, (n + 1)%7 ? " directory" : " document")
#define __printPostfixDirectoryType(n) printf((n + 1)%7 ? "directory" : "document")


#define DirAscii __DirType_ascii_codes + __DirType_ascii_code_stride * 
#define __DirType_ascii_code_stride 8
extern char __DirType_ascii_codes[(__DirType_ascii_code_stride + 1) * 7];

#define __enable_DirType_ascii_code(n) printf(DirAscii n)
#define __disable_DirType_ascii_codes printf("\033[0m")

// END SEGMENT : UI






#endif