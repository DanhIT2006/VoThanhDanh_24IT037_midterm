#include "file_entry.h"
#include <stdio.h>
#include <string.h>

int get_file_entry(const char *dir, const char *name, FileEntry *entry) {
    strncpy(entry->name, name, sizeof(entry->name) - 1);
    if (strcmp(dir, ".") == 0 || strcmp(dir, "") == 0) {
        snprintf(entry->path, sizeof(entry->path), "%s", name);
    } else {
        snprintf(entry->path, sizeof(entry->path), "%s/%s", dir, name);
    }
    return lstat(entry->path, &entry->st);
}