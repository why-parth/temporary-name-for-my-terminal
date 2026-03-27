#ifndef PRINT_DL_HEADER
#define PRINT_DL_HEADER

#include "macro_DL(path).h"

void __printDL(dl dlname);


void __showDL(dl dlname) {
    linef("[_/] %s", dlname.path);
    indent += 5;
    if (dlname.files)
    for (fileptr toprint = dlname.files; toprint; toprint = toprint->next)
        wlinef(L"%s %s", toprint->type is_directory ? L"[_/]" : L"|_=|", toprint->name);
    indent = 0;
}

void __printDL(dl dlname) {
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



#endif