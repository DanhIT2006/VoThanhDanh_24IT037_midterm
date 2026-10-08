#ifndef LS_H
#define LS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

typedef struct {
    int A, a, C, d, F, f, h, i, k, l, n, q, R, r, S, s, t, u, w;
} LsFlags;

extern LsFlags flags;

void list_directory(const char *path);
void print_file_info(const char *path, const char *filename, const struct stat *st);
int compare_entries(const void *a, const void *b);

#endif