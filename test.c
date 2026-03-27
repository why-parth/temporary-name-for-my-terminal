#include <stdio.h>

#include "macro_linef(format).h"

#include "printdl.h"


int main (void) {
    
    dl a = DL(".");

    fileptr exp = __search_in_directory(a, "exp");
    if (exp) __convert_file_to_dl(a.path, exp);

    fileptr exp1 = __search_in_directory(a, "exp1");
    if (exp1) __convert_file_to_dl(a.path, exp1);

    fileptr exp2 = __search_in_directory(a, "exp2");
    if (exp2) __convert_file_to_dl(a.path, exp2);

    __printdl(a);
        

    freedl(a);

    return 0;
}



