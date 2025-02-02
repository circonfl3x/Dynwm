#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wchar.h>
#include <unistd.h>
#include <locale.h>
#include "main.h"

int main(int argc, char **argv){
    setlocale(LC_CTYPE, "");
    char *fpath;
    if(argc < 2){
        Error("No config file provided", NULL);
        return 1;
    }
    else fpath = argv[1];

    FILE *file = fopen(fpath, "r");

    if(!file){
        Error("Filepath '",file,"' cannot be found!",NULL);
        return 1;
    }
    walls = malloc(sizeof(warr));
    warr_new(walls);
    if(parse_config(file) == ERR){
        return 1; //TODO: This leaks memory
    }
    fclose(file);
    
    while(false){
        sleep(1);
    }
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    
    check_fpaths(walls);
    while(true){
        wallpaper *wall = return_closest(walls);
        if(wall){
            if(wall->UUID == UUID) ;
            else{
                //TODO: Waaaay too many allocations
                wchar_t *ct = concat(commandb.left,L" ", wall->fpath,L" ",commandb.right,NULL);
                char *comm = wstos(ct);  
                printf("%s\n", comm);
                int sys = system(comm);
                if(sys == 1){
                    Error("Couldn't run command '", comm,"'",NULL);
                }
                UUID = wall->UUID;
                free(ct);
                free(comm);
            }
        }
        #ifdef __TIMEOUT
            sleep(__TIMEOUT);
        #else
        sleep(5);
        #endif 
    } 
    warr_free(walls);  
     

    return 0;
}
