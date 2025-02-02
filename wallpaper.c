#include "main.h"
#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>
#include <stdint.h>
#include <limits.h>
int warr_new(warr *wp){
    
    wp->walls = NULL;
    wp->size = 0;
    return SUC;
}
int warr_push(warr *wp, wallpaper *wall){
    wp->size++;
    wp->walls = realloc(wp->walls, wp->size*sizeof(wallpaper));
    if(!wp->walls){
        Error("Cannot realloc", NULL);
        return ERR;
    }
    wp->walls[wp->size-1].fpath = wall->fpath;
    wp->walls[wp->size-1].time = wall->time;
    wp->walls[wp->size-1].UUID = wall->UUID;
    return SUC;
}
int warr_free(warr *wp){
    for(int i = 0; i < wp->size; i++){
        free(wp->walls[i].fpath);
        free(wp->walls[i].time);
    }
    free(wp->walls);
    return SUC;
}
int is_space(const wchar_t *line){
    const wchar_t *ptr = line;
    int ret = 0;
    bool issp = true;
    while(*ptr != '\0'){
       if(!iswspace(*ptr)){
           return ret;

       }else ret++; 
       ptr++;
    }
    if(issp == true) return -1;
    return ret;

}
int breakdown_cmd(const wchar_t *command){
    wchar_t *wcs = wcsstr(command, L"${}");
    if(wcs == NULL){
        Warning("No \033[35m${}\033[0m substitution in string. Wallpaper name won't be substituted into command",NULL);
        return ERR;
    }
    wchar_t *dup = wcsdup(command);
    wchar_t *ptr;
    wchar_t *tok = wcstok(dup,L"${}",&ptr);
    commandb.left = calloc(wcslen(tok)+1, sizeof(wchar_t));
    wcscpy(commandb.left,tok); 
    tok = wcstok(NULL,L"${}",&ptr); 
    if(!tok){
        commandb.right = NULL;
    }else{
        commandb.right = calloc(wcslen(tok)+1, sizeof(wchar_t)); 
        wcscpy(commandb.right, tok); 
    }
    return SUC;
}
int parse_line(const wchar_t *line){

    const wchar_t *ptr = line;
    wchar_t *dup = wcsdup(line);
    if(*ptr == ':'){
        ptr++;
        wchar_t *tmp;
        wchar_t *tok = wcstok(dup, L"\"", &tmp);
        bool err = false;
        
        wchar_t *commd = wcsdup(tok); //get command
        wchar_t *cmd = commd;
        cmd++;
        
        if(wcscmp(cmd, L"begin\n") == 0){ 
            if(check_flag(DYNAMIC) == 0){
               Warning("Dynamic wallpapers won't be loaded... not in dynamic mode", NULL); 
            }else puts("Loading dynamic wallpapers...");
            read_walls = true;
        }else if(wcscmp(cmd, L"end") == 0 || wcscmp(cmd, L"end\n") == 0){
           read_walls = false; 
        }
        else{
            qtrim(cmd);
            tok = wcstok(NULL, L"\"", &tmp);
            if(tok == NULL){
                Error("Command '",cmd,"' needs content");
                err = true;
            }
            wchar_t *content = wcsdup(tok);
            if(wcscmp(cmd, L"cmd") == 0){
               breakdown_cmd(content);

            }
            else if(wcscmp(cmd, L"type") == 0){
                if(wcscmp(content,L"Dynamic") == 0){
                    if(check_flag(LIGHTDARK) == 1){
                        Error("Cannot enable 'Dynamic' mode when 'Theme' mode is already enabled",NULL);
                        return ERR;
                    }
                    enable_flag(DYNAMIC);
                }else if(wcscmp(content, L"Theme")){
                    if(check_flag(DYNAMIC) == 1){
                        Error("Cannot enable 'Theme' mode when 'Dynamic' mode is already enabled", NULL); 
                        return ERR;
                    }
                    enable_flag(LIGHTDARK);
                    Error("'Theme' mode isn't implemented yet",NULL);
                    return ERR;
                }else{
                    Error("Uknown mode '",content,"'",NULL);
                    return ERR;
                } 
            }else if(wcscmp(cmd, L"light") == 0){
                if(check_flag(DYNAMIC) == 1){
                    Warning("Light and dark theme wallpapers will be ignored when using 'Dynamic' mode",NULL);
                }
            }else if(wcscmp(cmd, L"dark") == 0){
                if(check_flag(LIGHTDARK)==1){
                    
                    Warning("Light and dark theme wallpapers will be ignored when using 'Dynamic' mode",NULL);
                }
            }
            free(commd);
            free(content);
        }
    }

    else if(read_walls == true){
        wchar_t *tmp;
        wchar_t *tok  = wcstok(dup, L"=",&tmp);
        wchar_t *ttime = wcsdup(tok);
        tok = wcstok(NULL, L"=", &tmp);
        if(tok == NULL){
            Error("Need to give a filepath after '=' sign",NULL);
            return ERR;
        }
        wchar_t *fpath = wcsdup(tok);
        qtrim(fpath);
        wallpaper *wall = malloc(sizeof(wallpaper));
        wall->fpath = wcsdup(fpath);
        wall->time = parse_time(ttime);
        if(wall->time == NULL) return ERR; //TODO: Fix memory leak here
        wall->UUID = (rand()%INT_MAX); //if it generates the same UUID as another wallahi
        printf("%ls at time %ls\n", wall->fpath, ttime);
        warr_push(walls, wall);
        free(wall);
        free(ttime);
        free(fpath);
    }
    else{

    }
    free(dup);
    return SUC;
}

int parse_config(FILE *file){
    wchar_t buff[1024];
    while(fgetws(buff,1024,file)){
        wchar_t *ptr = buff;
        // printf("%ls", ptr);
        int offset = is_space(ptr);
        if(offset == -1) continue;
        else{
            ptr += offset;
            // printf("%ls", ptr);
            if(parse_line(ptr) == ERR) return ERR;
        }
    }
    return SUC;
}

wallpaper *is_time(warr *walls){
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    for(int i = 0; i < walls->size; i++){
        if(walls->walls[i].time->tm_hour == t->tm_hour){
            if(walls->walls[i].time->tm_min == t->tm_min){
                return &walls->walls[i];
            }
        }
    }
    return NULL;
}

wallpaper *return_closest(warr *walls){
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    wallpaper *w = NULL;
    int hour_diff = 100;
    int min_diff = 100;

    for(int i = 0; i < walls->size; i++){
        struct tm *wtime = walls->walls[i].time;
        int hdiff,mdiff;

        if((hdiff = t->tm_hour - wtime->tm_hour) >= 0 && hdiff < hour_diff){
            if(hdiff == 0){
                if((mdiff = t->tm_min) >= 0 && mdiff < min_diff){
                    hour_diff = hdiff;
                    min_diff = mdiff;
                    w = &walls->walls[i];

                }
            }else{
                min_diff = 100;
                hour_diff = hdiff;
                w = &walls->walls[i];
            }
        }
       
    }
    if(w == NULL){
        t->tm_hour = 23;
        t->tm_min = 59;
        for(int i = 0; i < walls->size; i++){
            struct tm *wtime = walls->walls[i].time;
            int hdiff,mdiff;

            if((hdiff = t->tm_hour - wtime->tm_hour) >= 0 && hdiff < hour_diff){
                if(hdiff == 0){
                    if((mdiff = t->tm_min) >= 0 && mdiff < min_diff){
                        hour_diff = hdiff;
                        min_diff = mdiff;
                        w = &walls->walls[i];

                    }
                }else{
                    min_diff = 100;
                    hour_diff = hdiff;
                    w = &walls->walls[i];
                }
        }
       
    }
    }
    return w;

}
