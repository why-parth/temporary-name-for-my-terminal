#include "macro_DL(path).h"

dl ** __active_DL_ptr_ptr = &__active_DL_ptr;

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
    filename_datatype retname = ret->name;
    for (int i = 0; *(file_name) && i < dl_file_name_length ; i++) *(retname++) = *(file_name++); retname[0] = '\0';
    ret->type = file_type;
    ret->next = NULL;
    return ret;
}

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
    if (!(wstr = (wchar_t *)malloc((len + 1) * sizeof(wchar_t)))) return NULL;

    mbstowcs(wstr, mb_str, len + 1);

    return wstr;
}

void __os_independent_file_handling(dl * directory_list, file_datatype file) {
        __append_dl(directory_list, file filename, file filetype);
}

dl __get_dl(const char * directory) {
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
    struct dirent * file;
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
    if (!files) return;
    fileptr file = files, to_del;
    while (file->next != NULL) to_del = file, file = file->next, free(to_del);
    free(file);
}

void __del_dl(dl * directory_list) {
    __del_dl_files(directory_list->files); directory_list->files = NULL;
    directory_list->n_files = 0;
    for (int i = 0; i < cwd_max_len; i++) directory_list->path[i] = 0;
}
