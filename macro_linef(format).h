#ifndef SHOWLINE_HEADER
#define SHOWLINE_HEADER
// Just for the UI, not related to any logic.

#include <stdio.h>
#include <string.h>


long long int __linef_length = 0;

unsigned int indent = 0;

#define indent_mode_char    0x01
#define indent_mode_str     0x02
#define indent_mode_func    0x04

#define default_indent_char ' '

unsigned int __print_indentation(unsigned int i) {
    if (!i) i = indent;
    
    unsigned int ret = 0;
    while (i) {

        #if !defined(indent_mode)
        putchar(default_indent_char);

        #elif (indent_mode & indent_mode_char)
        putchar(indent_char);

        #elif (indent_mode & indent_mode_str)
        ret += printf(indent_str);

        #elif (indent_mode & indent_mode_func)
        ret += indent_char_func();
        
        #endif

        i--;
    }

    #if !defined(indent_mode) || (indent_mode & indent_mode_char)
    return i;

    #elif (indent_mode & indent_mode_str)
    return i * strlen(indent_str)

    #elif (indent_mode & indent_mode_func)
    return ret;
    
    #endif
}

#define ascii_color_code(n) printf("\033[%dm", n)
#define ascii_color_code2(a, b) printf("\033[%d;%dm", a, b)
#define ascii_color_code3(a, b, c) printf("\033[%d;%d;%dm", a, b, c)

#define linef putchar(__linef_length ? '\n' : '\0'), __linef_length = __print_indentation(indent) + printf
#define wlinef putchar(__linef_length ? '\n' : '\0'), __linef_length = __print_indentation(indent) + wprintf


#define err(text) putchar(__linef_length ? '\n' : '\0'), __linef_length = __print_indentation(indent) + printf("\033[31m%s\033[0m", text)
#define erri(text) putchar(__linef_length ? '\n' : '\0'), __linef_length = __print_indentation(indent) + printf("\033[31;3m%s\033[0m", text)
#define errb(text) putchar(__linef_length ? '\n' : '\0'), __linef_length = __print_indentation(indent) + printf("\033[31;1m%s\033[0m", text)
#define errib(text) putchar(__linef_length ? '\n' : '\0'), __linef_length = __print_indentation(indent) + printf("\033[31;1;3m%s\033[0m", text)


#endif