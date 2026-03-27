/*
Types of Directories:

Command Directory (CD)
    has 'main.c' in it.

Command Implicit Sequence Directory (CISD)
    has 'main.c' and 'seq.txt' in it.

Command Explicit Sequence Directory (CESD)
    has 'seq.c' in it.

Command Line Directory (CLD)
    has one of more CD, CISD or CESD in it.

Regular Directory (RD)
    else case.


Syntax of calling one command:
    if face in directory:
        face cmd ...
    if inside the face terminal:
        cmd ...

How is a command ran?
    When 'cmd ...' (or 'face cmd ...') is ran, the directory named 'cmd' must be present in the CLD (you can manually change the CWD to CLD if the terminal is no opened in the CLD itself).
    
    If the directory named 'cmd' is present, if its a valid command (directory is of type CD, CISD or CESD), it 'compiles that directory'. else 'cmd is not a valid command' is printed to the terminal.
    
    'compiles that directory' varries for each type.

    For CD:
        The main.c is compiled and the compiled runnable file is simply ran.
    
    For CISD:
        The main.c is compiled, ran and then, one-by-one, the lines of seq.txt is ran as a command in the terminal.
        
        'seq.txt' looks like this:
            filename0 ... : ...
            filename1 ... : ...
            filename2 ... : ...
            ...
        
        main.c acts as a default compilation and run. After that, for each line in seq.txt, the texts before ':' are treated as 'files to be compiled' (filename = name of the first file) and thus is compiled (gcc filename.c ... -o filename). then the text after ':' is treated as 'text to be passed as command line arguments' thus the filename executable is ran (filename ...).
    
    For CESD:
        There is no main.c to initialise anything, the process begins with the first line of the seq.txt file.

*/

#ifndef DIRECTORY_COMMAND_HEADER
#define DIRECTORY_COMMAND_HEADER


#include "dirtype.h"
#include <string.h>

int filename_cmp(filename_datatype name, char * is);

fileptr __main_in_directory(dl dlname);

fileptr __seq_in_directory(dl dlname);

fileptr __search_in_directory(dl dlname, char * name);

fileptr __search_in_directory_by_index(dl dlname, size_t index);

unsigned int __dir_type(dl dlname);

// makes DL for all sub-directories in dlname and stores their pointer in the (dl file object) fileptr of each file
void __convert_subdirectories_to_dl(dl dlname);

// makes DL for the passed index of the DL
int __convert_index_to_dl(dl dlname, unsigned int index);

// makes DL for the passed fileptr passed to it
int __convert_file_to_dl(const char * path, fileptr file);


#endif