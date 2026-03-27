#include <stdio.h>

#include "macro_linef(format).h"

#include "macro_printDL(dlname).h"

#include "dircmd.h"

int main (void) {
    
    dl a = DL(".");


    __printDL(a);

    putchar('\n');
    
    fileptr mainfile = __directory_is_command(a);
    fileptr seqfile = __seq_in_directory(a);
    
    if (mainfile) wlinef(L"main file exists in the directory: '%s'", mainfile->name);
    else errb("main file does NOT exist in the directory");
    
    putchar('\n');

    if (seqfile) wlinef(L"seq file exists in the directory: '%s'", seqfile->name);
    else errb("seq file does NOT exist in the directory");
    
    putchar('\n');

    fileptr name0 = __get_from_directory(a, 6);
    fileptr name1 = __search_in_directory(a, "README.md");
    fileptr name2 = __search_in_directory(a, "abcd.efg");

    if (name0) wlinef(L"file at the index 6 is '%s'", name0->name); else erri("file name0 does not exist");
    if (name1) wlinef(L"file name1 exists : '%s'", name1->name); else erri("file name1 does NOT exist");
    if (name2) wlinef(L"file name2 exists : '%s'", name2->name); else erri("file name2 does NOT exist");
    
    return 0;
}



