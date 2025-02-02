#pragma once
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <wchar.h>
#include <time.h>
#define Error(x,...) _Generic((x), char*:error_s,wchar_t*:error_ws)\
        (x, __VA_ARGS__)
#define Warning(x,...) _Generic((x), char*:Warning_s,wchar_t*:Warning_ws)\
        (x, __VA_ARGS__)
#define ERR 1
#define SUC 0

typedef enum {
    DYNAMIC,LIGHTDARK
}FLAGS;
typedef enum {
    OK,ISNEXIST,ISDIR,UNKNOWN
} FMODES;
typedef enum{
    JPG,PNG,BMP,GIF,ISNIMG
} IMGTYPE;
typedef struct{
    wchar_t *fpath;
    struct tm *time;
    unsigned int UUID; 
}wallpaper;
typedef struct{
    wallpaper *walls;
    unsigned int size;
}warr;

typedef struct{
    wchar_t *left;
    wchar_t *right;
} command_breakdown;

extern int flags;
extern bool read_walls;
extern FILE *file;
extern warr *walls;
extern command_breakdown commandb;
static int UUID;


extern void error_s(const char *s, ...);
extern void error_ws(const wchar_t *ws, ...);
extern void Warning_s(const char *s, ...);
extern void Warning_ws(const wchar_t *ws, ...);
//flags
extern void enable_flag(FLAGS f);
extern void disable_flag(FLAGS f);
extern bool check_flag(FLAGS f);

//config
extern wallpaper **config(); 

//wallpaper
extern int warr_new(warr *wp);
extern int warr_push(warr *wp, wallpaper *wall);
extern int warr_free(warr *wp);
//config
extern int parse_config(FILE *file);
extern struct tm *parse_time(const wchar_t *time);
extern bool check_fpaths(warr *wp);
//extras
extern void qtrim(wchar_t *ws);
extern wchar_t *concat(const wchar_t *ws, ...);
extern char *wstos(const wchar_t *ws);
extern wallpaper *is_time(warr *walls);
extern wallpaper *return_closest(warr *walls);
