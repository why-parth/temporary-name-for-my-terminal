#ifndef DIRECTORY_COMMAND_HEADER
#define DIRECTORY_COMMAND_HEADER

#include "macro_DL(path).h"
#include "string.h"

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

fileptr __directory_is_command(dl dlname) {

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

fileptr __get_from_directory(dl dlname, size_t index) {

    fileptr checkfile = dlname.files;

    for (; index && checkfile; index--) checkfile = checkfile->next;

    if (checkfile) return checkfile;
    else return NULL;
}

#endif