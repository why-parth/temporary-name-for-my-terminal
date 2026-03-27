#include <stdio.h>

#include "macro_linef(format).h"

#include "printdl.h"

#include "dircmd.h"

int main (void) {
    
    dl a = DL(".");

    __printdl_raw(a);

    fileptr exp = __search_in_directory(a, "exp");

    if (!exp) err("exp does not exist!");
    else {
        linef("the directory exp exist!");
        __convert_file_to_dl(a.path, exp);
        __printdl_raw(*(exp->dl));
    }
        

    freedl(a);
    return 0;
}



