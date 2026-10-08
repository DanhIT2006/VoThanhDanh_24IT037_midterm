#include "ls.h"

typedef struct {
    char name[256];
    struct stat st;
} FileEntry;

void print_permissions(mode_t mode) {
    char type = '-';
    if (S_ISDIR(mode)) type = 'd';
    else if (S_ISLNK(mode)) type = 'l';
    else if (S_ISCHR(mode)) type = 'c';
    else if (S_ISBLK(mode)) type = 'b';
    else if (S_ISFIFO(mode)) type = 'p';
    else if (S_ISSOCK(mode)) type = 's';
    
    printf("%c", type);
    printf((mode & S_IRUSR) ? "r" : "-");
    printf((mode & S_IWUSR) ? "w" : "-");
    printf((mode & S_ISUID) ? ((mode & S_IXUSR) ? "s" : "S") : ((mode & S_IXUSR) ? "x" : "-"));
    printf((mode & S_IRGRP) ? "r" : "-");
    printf((mode & S_IWGRP) ? "w" : "-");
    printf((mode & S_ISGID) ? ((mode & S_IXGRP) ? "s" : "S") : ((mode & S_IXGRP) ? "x" : "-"));
    printf((mode & S_IROTH) ? "r" : "-");
    printf((mode & S_IWOTH) ? "w" : "-");
    printf((mode & S_ISVTX) ? ((mode & S_IXOTH) ? "t" : "T") : ((mode & S_IXOTH) ? "x" : "-"));
}

void print_name_with_F_flag(const char *name, mode_t mode) {
    printf("%s", name);
    if (flags.F) {
        if (S_ISDIR(mode)) printf("/");
        else if (S_ISLNK(mode)) printf("@");
        else if (S_ISSOCK(mode)) printf("=");
        else if (S_ISFIFO(mode)) printf("|");
        else if (mode & S_IXUSR || mode & S_IXGRP || mode & S_IXOTH) printf("*");
    }
    printf("\n");
}

void print_file_info(const char *path, const char *filename, const struct stat *st) {
    if (flags.i) {
        printf("%llu ", (unsigned long long)st->st_ino);
    }
    
    if (flags.s) {
        long blocks = st->st_blocks;
        if (flags.k) blocks /= 2;
        printf("%ld ", blocks);
    }

    if (flags.l || flags.n) {
        print_permissions(st->st_mode);
        printf(" %lu ", (unsigned long)st->st_nlink);

        if (flags.n) {
            printf("%u %u ", st->st_uid, st->st_gid);
        } else {
            struct passwd *pw = getpwuid(st->st_uid);
            struct group *gr = getgrgid(st->st_gid);
            printf("%s %s ", pw ? pw->pw_name : "unknown", gr ? gr->gr_name : "unknown");
        }

        if (flags.h) {
            // Human readable size logic[cite: 1]
            if (st->st_size > 1024 * 1024 * 1024) printf("%.1fG ", (float)st->st_size / (1024*1024*1024));
            else if (st->st_size > 1024 * 1024) printf("%.1fM ", (float)st->st_size / (1024*1024));
            else if (st->st_size > 1024) printf("%.1fK ", (float)st->st_size / 1024);
            else printf("%lld ", (long long)st->st_size);
        } else {
            printf("%lld ", (long long)st->st_size);
        }

        char time_str[20];
        time_t t = flags.u ? st->st_atime : (flags.C ? st->st_ctime : st->st_mtime);
        struct tm *tm_info = localtime(&t);
        strftime(time_str, sizeof(time_str), "%b %e %H:%M", tm_info);
        printf("%s ", time_str);
    }
    
    print_name_with_F_flag(filename, st->st_mode);
}

int compare_entries(const void *a, const void *b) {
    FileEntry *fa = (FileEntry *)a;
    FileEntry *fb = (FileEntry *)b;
    
    int result = 0;
    if (flags.S) {
        result = (fb->st.st_size - fa->st.st_size);
    } else if (flags.t) {
        time_t time_a = flags.u ? fa->st.st_atime : fa->st.st_mtime;
        time_t time_b = flags.u ? fb->st.st_atime : fb->st.st_mtime;
        result = (time_b - time_a);
    } else {
        result = strcmp(fa->name, fb->name);
    }

    return flags.r ? -result : result;
}

void list_directory(const char *path) {
    DIR *dir = opendir(path);
    if (!dir) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    FileEntry *entries = NULL;
    int count = 0;
    int capacity = 10;
    entries = malloc(capacity * sizeof(FileEntry));

    long total_blocks = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (!flags.a && !flags.A && entry->d_name[0] == '.') continue;
        if (flags.A && (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)) continue;

        if (count >= capacity) {
            capacity *= 2;
            entries = realloc(entries, capacity * sizeof(FileEntry));
        }

        char fullpath[1024];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);
        
        strncpy(entries[count].name, entry->d_name, 255);
        if (lstat(fullpath, &entries[count].st) == 0) {
            total_blocks += entries[count].st.st_blocks;
        }
        count++;
    }
    closedir(dir);

    if ((flags.l || flags.s) && count > 0) {
        long d_blocks = total_blocks;
        if (flags.k) d_blocks /= 2;
        printf("total %ld\n", d_blocks);
    }

    if (!flags.f) {
        qsort(entries, count, sizeof(FileEntry), compare_entries);
    }

    for (int i = 0; i < count; i++) {
        char fullpath[1024];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entries[i].name);
        print_file_info(path, entries[i].name, &entries[i].st);
    }

    if (flags.R) {
        for (int i = 0; i < count; i++) {
            if (S_ISDIR(entries[i].st.st_mode) && 
                strcmp(entries[i].name, ".") != 0 && 
                strcmp(entries[i].name, "..") != 0) {
                char fullpath[1024];
                snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entries[i].name);
                printf("\n%s:\n", fullpath);
                list_directory(fullpath);
            }
        }
    }

    free(entries);
}