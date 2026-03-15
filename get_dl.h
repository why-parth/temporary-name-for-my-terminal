#ifndef GET_DL_HEADER
#define GET_DL_HEADER

#define AUTO_INITIALISE_HEADER
#include "get_cwd.h"

#define dl_file_name_length 32

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>

#ifdef _WIN32

#include <windows.h>
#define filename_datatype WCHAR *
#define filename_char_datatype WCHAR
#define filetype_datatype DWORD
#define filetype .dwFileAttributes
#define filename .cFileName
#define is_directory & FILE_ATTRIBUTE_DIRECTORY

#else

#include <dirent.h>
#define filename_datatype char *
#define filename_char_datatype char
#define filetype_datatype unsigned char
#define filetype ->d_type
#define filename ->d_name
#define is_directory == DT_DIR

#endif

typedef struct dl_file {
    filename_char_datatype name[dl_file_name_length];
    filetype_datatype type;
    struct dl_file * next;
} * fileptr;

#define Type .type

typedef struct dl {
    char path[cwd_max_len];
    fileptr files;
    unsigned int n_files;
} dl;

dl make_dl(const char * path) {
    dl ret;
    ret.files = NULL;
    ret.n_files = 0;

    char * retpath = ret.path; 

    if (path[0] == '.') {
        pastecwd(ret.path);
        while(*(++retpath)); path++;
    }

    for (; *(path) ;) *(retpath++) = *(path++); retpath[0] = '\0';
    return ret;
}

fileptr make_dl_file(filename_datatype file_name, filetype_datatype file_type) {
    fileptr ret = (fileptr)malloc(sizeof(struct dl_file));
    filename_datatype retname = ret->name; for (; *(file_name) ;) *(retname++) = *(file_name++); retname[0] = '\0';
    ret->type = file_type;
    ret->next = NULL;
}

#define append_dl(dlname, fname, ftype) __append_dl(&dlname, fname, ftype)
void __append_dl(dl * directory_list, filename_datatype file_name, filetype_datatype file_type) {
    fileptr to_append = directory_list->files;
    if (to_append == NULL) {
        to_append = make_dl_file(file_name, file_type);
        directory_list->files = to_append;
    }
    else {
        while (to_append->next != NULL) to_append = to_append->next;
        to_append->next = make_dl_file(file_name, file_type);
    }
    directory_list->n_files++;
}

wchar_t * mb_to_wc (const char * mb_str) {
    if (!mb_str) return NULL;

    setlocale(LC_ALL, "");

    size_t len = mbstowcs(NULL, mb_str, 0);
    if (len == ((size_t) - 1)) return NULL;

    wchar_t * wstr;
    if (!(wstr = malloc((len + 1) * sizeof(wchar_t)))) return NULL;

    mbstowcs(wstr, mb_str, len + 1);

    return wstr;
}

#define osi_fh(dlname, file) __os_independent_file_handling(&dlname, file)
void __os_independent_file_handling(dl * directory_list, WIN32_FIND_DATAW file) {
        __append_dl(directory_list, file filename, file filetype);
}

dl get_dl(const char * directory) {
    dl ret = make_dl(directory);

    #ifdef _WIN32

    wchar_t * directory_wc = mb_to_wc(directory);

    wchar_t cwdbuffer[cwd_max_len];
    _snwprintf(cwdbuffer, cwd_max_len, L"%ls\\*", directory_wc);

    free(directory_wc);

    WIN32_FIND_DATAW file;
    HANDLE handle = FindFirstFileW(cwdbuffer, &file);
    if (handle != INVALID_HANDLE_VALUE) {
    do
    {

        __os_independent_file_handling(&ret, file);

    }
    while (FindNextFileW(handle, &file));
    FindClose(handle);
    }
    
    #else
    
    DIR *dir = opendir(".");
    struct dirent *file;
    if (dir)
    {
        while ((file = readdir(dir)) != NULL) {
            
        __os_independent_file_handling(&ret, file);

    }
    closedir(dir);
    }

    #endif

    return ret;
}

void __del_dl_files(fileptr files) {
    fileptr file = files, to_del;
    while (file->next != NULL) to_del = file, file = file->next, free(to_del);
    free(file);
}

#define freedl(dlname) __del_dl(&dlname)
void __del_dl(dl * directory_list) {
    __del_dl_files(directory_list->files); directory_list->files = NULL;
    directory_list->n_files = 0;
    for (int i = 0; i < cwd_max_len; i++) directory_list->path[i] = 0;
}

#endif