#include <stdio.h>

#include "get_dl.h"

int main (void) {

    // Creating a DL (Directory List) A
    dl A = get_dl(".");

    // Seeing if the DL is succesfully made or not
    wprintf(L"name of the file 3 out of %d is \"%ls\"", A.n_files, A.files->next->next->name);
    // >>> name of the file 3 out of 9 is ".git"

    // freeing the DL A
    freedl(A);
    
    return 0;
}