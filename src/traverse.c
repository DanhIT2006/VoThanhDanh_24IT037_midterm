#include "traverse.h"
#include "file_entry.h"
#include "sort.h"
#include "display.h"
#include "options.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

void traverse_directory(const char *path, int print_dir_name) {
    DIR *dir = opendir(path);
    if (!dir) { perror(path); return; }

    if (print_dir_name) printf("\n%s:\n", path);

    int capacity = 10, count = 0;
    FileEntry *entries = malloc(capacity * sizeof(FileEntry));
    struct dirent *dp;

    while ((dp = readdir(dir)) != NULL) {
        if (!flags.a && !flags.f && !flags.A && dp->d_name[0] == '.') continue;
        if (flags.A && !flags.a && !flags.f && (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)) continue;

        if (count >= capacity) {
            capacity *= 2;
            entries = realloc(entries, capacity * sizeof(FileEntry));
        }
        get_file_entry(path, dp->d_name, &entries[count++]);
    }
    closedir(dir);

    sort_entries(entries, count);
    if (flags.l || flags.s || flags.n) print_total_blocks(entries, count);

    for (int i = 0; i < count; i++) print_file(&entries[i]);

    if (flags.R) {
        for (int i = 0; i < count; i++) {
            if (S_ISDIR(entries[i].st.st_mode) && strcmp(entries[i].name, ".") != 0 && strcmp(entries[i].name, "..") != 0) {
                traverse_directory(entries[i].path, 1);
            }
        }
    }
    free(entries);
}

void handle_arguments(int argc, char *argv[]) {
    if (optind == argc) {
        traverse_directory(".", 0);
    } else {
        for (int i = optind; i < argc; i++) {
            if (flags.d) {
                FileEntry entry;
                if (get_file_entry(".", argv[i], &entry) == 0) print_file(&entry);
            } else {
                FileEntry entry;
                get_file_entry(".", argv[i], &entry);
                if (S_ISDIR(entry.st.st_mode)) {
                    traverse_directory(argv[i], argc - optind > 1);
                } else {
                    print_file(&entry);
                }
            }
        }
    }
}