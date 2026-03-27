#ifndef PRINT_DL_HEADER
#define PRINT_DL_HEADER

#include "dirlist.h"
#include "macro_linef(format).h"


void __printdl_raw(dl dlname) {
    linef("\033[1m%s\033[0m", dlname.path);
    indent += 2;
    if (dlname.files)
        for (fileptr toprint = dlname.files; toprint; toprint = toprint->next)
            if (toprint->type is_directory)
                wlinef(L"\033[1m%s\033[0m", toprint->name);
            else
                wlinef(L"\033[3m%s\033[0m", toprint->name);
    indent = 0;
}

void __printdl_color(dl dlname) {
    __enable_DirType_ascii_code(dirtype_dlparsed);
    linef(dlname.path);
    __disable_DirType_ascii_codes;
    indent += 2;
    unsigned int dirtype = 0;
    if (dlname.files)
        for (fileptr toprint = dlname.files; toprint; toprint = toprint->next) {

            __enable_DirType_ascii_code(toprint->type);

            wlinef(toprint->name);

            __disable_DirType_ascii_codes;
        }

    indent = 0;
}


void __printdl(dl dlname) {
    __enable_DirType_ascii_code(dirtype_dlparsed);
    linef(dlname.path);
    __disable_DirType_ascii_codes;
    indent += 2;
    unsigned int dirtype = 0;
    if (dlname.files)
        for (fileptr toprint = dlname.files; toprint; toprint = toprint->next) {

            __enable_DirType_ascii_code(toprint->type);

            linef("  ");
            __printDirectoryType(toprint->type);
            printf("  ");
            wprintf(toprint->name);

            __disable_DirType_ascii_codes;
        }

    indent = 0;
}



#endif