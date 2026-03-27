#ifndef GET_DL_HEADER
#define GET_DL_HEADER

/* Interface Definitions */
#define dl_file_name_length 32

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

/* Structures */

// fileptr : used for storing files' name, type and the pointer-to next file (thus, it can be used to form a linked list)
typedef struct dl_file {
    filename_char_datatype name[dl_file_name_length + 1];
    filetype_datatype type;
    struct dl_file * next;
} * fileptr;

// dl : used for storing and managing directories 
typedef struct dl {
    char path[cwd_max_len + 1];
    fileptr files;
    unsigned int n_files;
} dl;


/* Dynamic DL Use */
dl * __active_DL_ptr;
extern dl ** __active_DL_ptr_ptr;
#define __define_DL_function(id) { *__active_DL_ptr_ptr = id; return *((dl *)id); }

/* Functions */

// returns an empty dl with its path set to what is given
dl make_dl(const char * path);

// returns a dl_file that stores essential information (file_name and file_type) and has its fileptr next set to NULL
fileptr make_dl_file(filename_datatype file_name, filetype_datatype file_type);

// append_dl pushes a dl_file that stores essential information (fname and ftype) at the last fileptr (linking the nodes) and updates n_files
#define append_dl(dlname, fname, ftype) __append_dl(&dlname, fname, ftype)
void __append_dl(dl * directory_list, filename_datatype file_name, filetype_datatype file_type);

// converts a multibyte string to a wide character string
wchar_t * mb_to_wc (const char * mb_str);

// osi_fh automates the process of appending a file to a dl
#define osi_fh(dlname, file) __os_independent_file_handling(&dlname, file)
void __os_independent_file_handling(dl * directory_list, file_datatype file);

// DL takes in a path and returns the respective DL of the directory that exists at that path
#define DL(path) __get_dl(path)
dl __get_dl(const char * directory);

// deletes the fileptr that is passed to it along with all the fileptr that are attached to it (in the next direction)
void __del_dl_files(fileptr files);

// frees the dl completely
#define freedl(dlname) __del_dl(&dlname)
void __del_dl(dl * directory_list);

#endif