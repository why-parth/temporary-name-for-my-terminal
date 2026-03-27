#include "dirtype.h"

char __DirType_identifiers[] = "\033[1;35mterminal\033[0m\0\033[1;32mexplicit\033[0m\0\033[1;32mimplicit\033[0m\0\033[1;32mmainfile\033[0m\0\033[1;34mdlparsed\033[0m\0\033[0;33munparsed\033[0m\0\033[3mdocument\033[0m\0";

char __DirType_ascii_codes[] = "\033[1;35m\0\033[1;32m\0\033[1;32m\0\033[1;32m\0\033[1;34m\0\033[0;33m\0\033[3m\0";


#include "dirlist.h"

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
    ret->__filetype = file_type;
    ret->next = NULL;
    ret->dl = NULL;
    ret->type = (file_type is_directory) ? 5 : 6;
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
    while (file->next != NULL) to_del = file, file = file->next,
        free(to_del->dl),
        free(to_del);
    free(file);
}

void __del_dl(dl * directory_list) {
    if (!directory_list) return;
    __del_dl_files(directory_list->files); directory_list->files = NULL;
    directory_list->n_files = 0;
    for (int i = 0; i < cwd_max_len; i++) directory_list->path[i] = 0;
}

#include "dircmd.h"

int filename_cmp(filename_datatype name, char * is) {
    if (name == NULL) return 0;
    #ifdef _WIN32
    wchar_t * wcis = mb_to_wc(is);
    return !wcscmp(name, wcis);
    free(wcis);
    #else
    return !strcmp(name, is);
    #endif
}

fileptr __main_in_directory(dl dlname) {

    fileptr checkmain = dlname.files;

    for (; checkmain; checkmain = checkmain->next)
        if (filename_cmp(checkmain->name, "main.c"))
            return checkmain;
    
    return NULL;
}

fileptr __seq_in_directory(dl dlname) {

    fileptr checkseq = dlname.files;

    for (; checkseq; checkseq = checkseq->next)
            if (filename_cmp(checkseq->name, "seq.c"))
            return checkseq;
    
    return NULL;
}

fileptr __search_in_directory(dl dlname, char * name) {

    fileptr checkfile = dlname.files;

    for (; checkfile; checkfile = checkfile->next)
        if (filename_cmp(checkfile->name, name))
        return checkfile;
    
    return NULL;
}

fileptr __search_in_directory_by_index(dl dlname, size_t index) {

    fileptr checkfile = dlname.files;

    for (; index && checkfile; index--) checkfile = checkfile->next;

    if (checkfile) return checkfile;
    else return NULL;
}

unsigned int __dir_type(dl dlname) {
    fileptr mainfile = __main_in_directory(dlname);
    fileptr seqfile = __seq_in_directory(dlname);

    if (mainfile) return dirtype_mainfile;
    else if (mainfile && seqfile) return dirtype_implicit;
    else if (seqfile) return dirtype_explicit;
    else return dirtype_dlparsed;

}

void __convert_subdirectories_to_dl(dl dlname) {
    fileptr curr = dlname.files;
    for (; curr; curr = curr->next) {
        if (curr->dl) continue; 

        dl * dlptr = (dl *)malloc(sizeof(dl));
        char dlpath[cwd_max_len + dl_file_name_length + 1];
        strcpy(dlpath, dlname.path);

        int i = 0; for (; dlpath[i]; i++); dlpath[i] = '\\'; dlpath[i + 1] = '\0';

        #ifdef _WIN32
            char mbdlfilename[dl_file_name_length + 1];
            wcstombs(mbdlfilename, curr->name, dl_file_name_length + 1);
            strcat(dlpath, mbdlfilename);
        #else
            strcat(dlpath, curr->name);
        #endif

        (* dlptr) = DL(dlpath);
        curr->dl = dlptr;
        curr->type = __dir_type(* dlptr);
    }
}

int __convert_index_to_dl(dl dlname, unsigned int index) {
    fileptr curr = dlname.files;

    for (; index; index--, curr = curr->next) {
        if (!curr) return -1;
        if (curr->dl) 0;
    }

    if (!curr) return -1;

    dl * dlptr = (dl *)malloc(sizeof(dl));
    char dlpath[cwd_max_len + dl_file_name_length + 1];
    strcpy(dlpath, dlname.path);

    int i = 0; for (; dlpath[i]; i++); dlpath[i] = '\\'; dlpath[i + 1] = '\0';

    #ifdef _WIN32
        char mbdlfilename[dl_file_name_length + 1];
        wcstombs(mbdlfilename, curr->name, dl_file_name_length + 1);
        strcat(dlpath, mbdlfilename);
    #else
        strcat(dlpath, curr->name);
    #endif

    (* dlptr) = DL(dlpath);
    curr->dl = dlptr;
    curr->type = __dir_type(* dlptr);

    return 1;
}

int __convert_file_to_dl(const char * path, fileptr file) {
    if (!file) return -1;
    if (file->dl) return 0;

    dl * dlptr = (dl *)malloc(sizeof(dl));
    char dlpath[cwd_max_len + dl_file_name_length + 1];
    strcpy(dlpath, path);

    int i = 0; for (; dlpath[i]; i++); dlpath[i] = '\\'; dlpath[i + 1] = '\0';

    #ifdef _WIN32
        char mbdlfilename[dl_file_name_length + 1];
        wcstombs(mbdlfilename, file->name, dl_file_name_length + 1);
        strcat(dlpath, mbdlfilename);
    #else
        strcat(dlpath, curr->name);
    #endif

    (* dlptr) = DL(dlpath);
    file->dl = dlptr;
    file->type = __dir_type(* dlptr);

    return 1;
}
