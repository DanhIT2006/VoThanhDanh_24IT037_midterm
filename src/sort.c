#include "sort.h"
#include "options.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

static int compare_entries(const void *a, const void *b) {
    FileEntry *fa = (FileEntry *)a;
    FileEntry *fb = (FileEntry *)b;
    int result = 0;

    if (flags.S) {
        result = (fb->st.st_size > fa->st.st_size) - (fb->st.st_size < fa->st.st_size);
    } else if (flags.t) {
        time_t time_a = flags.u ? fa->st.st_atime : (flags.c ? fa->st.st_ctime : fa->st.st_mtime);
        time_t time_b = flags.u ? fb->st.st_atime : (flags.c ? fb->st.st_ctime : fb->st.st_mtime);
        result = (time_b > time_a) - (time_b < time_a);
        if (result == 0) result = strcmp(fa->name, fb->name);
    } else {
        result = strcmp(fa->name, fb->name);
    }

    return flags.r ? -result : result;
}

void sort_entries(FileEntry *entries, int count) {
    if (!flags.f) {
        qsort(entries, count, sizeof(FileEntry), compare_entries);
    }
}