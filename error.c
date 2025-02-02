#include "main.h"
void error_s(const char *s, ...){
    
    va_list args;
    printf("\033[31mError: \033[0m");
    for(va_start(args, s); s != NULL; s = va_arg(args, const char *)){
        printf("%s ",s); 
    }
    puts("");

}
void error_ws(const wchar_t *ws, ...){
    
    va_list args;
    printf("\033[31mError: \033[0m");
    for(va_start(args, ws); ws != NULL; ws = va_arg(args, const wchar_t *)){
        printf("%ls ",ws); 
    }
    puts("");

}
void Warning_s(const char *s, ...){

     va_list args;
    printf("\033[35mWarning: \033[0m");
    for(va_start(args, s); s != NULL; s = va_arg(args, const char *)){
        printf("%s ",s); 
    }
    puts("");
}
void Warning_ws(const wchar_t *ws, ...){
    
    va_list args;
    printf("\033[35mWarning: \033[0m");
    for(va_start(args, ws); ws != NULL; ws = va_arg(args, const wchar_t *)){
        printf("%ls ",ws); 
    }
    puts("");

}
