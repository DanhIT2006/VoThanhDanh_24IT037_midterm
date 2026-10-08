#ifndef FILE_ENTRY_H
#define FILE_ENTRY_H

#include <sys/stat.h>

typedef struct {
    char name[256];
    char path[1024];
    struct stat st;
} FileEntry;

int get_file_entry(const char *dir, const char *name, FileEntry *entry);

#endif