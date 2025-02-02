#include "main.h"
#include <wchar.h>
#include <wctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

void enable_flag(FLAGS f){
    flags ^= 1<<f;
}

void disable_flag(FLAGS f){
    flags &= 1<<f;
}

bool check_flag(FLAGS f){
    return flags & (1<<f);
}

void qtrim(wchar_t *ws){
    while(iswspace(*ws)){
        ws++;
    }
    size_t end = wcslen(ws)-1;
    
    while(!iswalpha(ws[end])){
        ws[end] = '\0';
        end--;
    }
}

struct tm *parse_time(const wchar_t *time){
    struct tm *tmm = malloc(sizeof(struct tm)); 
    wchar_t *dup = wcsdup(time);
    wchar_t *ptr,*tmp;
    wchar_t *tok = wcstok(dup, L":",&ptr);
    tmm->tm_hour = wcstoll(tok, NULL,10);
    if(tmm->tm_hour>23){
        Error("Time can only be in the range of 0-23",NULL);
        return NULL;
    }
    tok = wcstok(NULL, L":", &ptr);
    if(tok == NULL) tmm->tm_min = 0;
    else tmm->tm_min = wcstoll(tok, NULL,10);
    if(tmm->tm_min>59){
        Error("Can you not count minutes bro?",NULL);
        return NULL;
    }
    free(dup);
    return tmm;
    
}
char *wstos(const wchar_t *ws){
    char *ret = calloc(wcslen(ws)+1,1);
    int a = wcstombs(ret, ws, wcslen(ws)); 
    if(a == -1) return NULL;
    return ret;
}
FMODES isexist(const char *fpath){
    struct stat buffer;
    if(stat(fpath, &buffer) == 0){
        if(S_ISDIR(buffer.st_mode)) return ISDIR;
        else if(S_ISREG(buffer.st_mode)) return OK;
        else return UNKNOWN;
    }else return ISNEXIST;
}
IMGTYPE isimg(const char *fpath){
    FILE *f = fopen(fpath, "rb");
    if(!f){
        Error("Couldn't open file '",fpath,"'",NULL);
        return false;
    }
    unsigned char *buff = calloc(4,sizeof(unsigned char));
    fread(buff,sizeof(unsigned char),4,f);
    fclose(f);

    if(buff[0] == 0xFF && buff[1] == 0xD8){
        return JPG;
    }else if(buff[0] == 0x89 && buff[1] == 0x50 &&
            buff[2] == 0x4E && buff[3] == 0x47){
        return PNG;
    }else if(buff[0] == 0x47 && buff[1] == 0x49 &&
            buff[2] == 0x46){
        return GIF;
    }else if(buff[0] == 0x42 && buff[1] == 0x4D){
        return BMP;
    }else return ISNIMG;
    

    free(buff);
    
}
bool check_fpaths(warr *wp){
    for(int i = 0; i < wp->size; i++){
       char *fp = wstos(wp->walls[i].fpath);
       switch(isexist(fp)){
            case OK:
            break;

            case ISNEXIST:
                Error("File '",fp,"' doesn't exist.", NULL );
            break;

            case ISDIR:
                Error(fp, " isn't a file, but a directory.", NULL);
            break;
            
            default:
            break;

       }
    #ifdef __CHECKIMG 
        switch(isimg(fp)){
            case JPG:
                printf("%s is a jpg\n",fp);
            break;

            case PNG:
                printf("%s is a PNG\n", fp);
            break;

            case BMP:
                printf("%s is a BMP\n", fp);
            break;

            case GIF:
                printf("%s is a GIF\n", fp);
            break;

            default:
                Error("Uknown image type or not an image type",NULL);
                free(fp);
                return false;
            break;
        }
    #endif
       free(fp);
    }
    return true;

}


wchar_t *concat(const wchar_t *ws, ...){
    va_list args;
    wchar_t *ret = calloc(1,sizeof(wchar_t));
    if(!ret){
        Error("Couldn't alloc",NULL);
        return NULL;
    }
    size_t len = 0;
    for(va_start(args, ws); ws != NULL; ws = va_arg(args, const wchar_t *)){
        len += wcslen(ws);
        ret = realloc(ret, sizeof(wchar_t)*(len+1));
        if(!ret){
            Error("Couldn't realloc",NULL);
            return NULL;
        }
        wcscat(ret,ws); 
    }
    return ret;
}
