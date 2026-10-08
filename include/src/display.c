#include "display.h"
#include "options.h"
#include <stdio.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

void print_total_blocks(FileEntry *entries, int count) {
    long total = 0;
    for (int i = 0; i < count; i++) total += entries[i].st.st_blocks;
    if (flags.k) total /= 2;
    if (count > 0) printf("total %ld\n", total);
}

void print_file(FileEntry *entry) {
    if (flags.i) printf("%llu ", (unsigned long long)entry->st.st_ino);
    
    if (flags.s) {
        long blocks = entry->st.st_blocks;
        if (flags.k) blocks /= 2;
        printf("%ld ", blocks);
    }

    if (flags.l || flags.n) {
        mode_t m = entry->st.st_mode;
        printf("%c%c%c%c%c%c%c%c%c%c ",
            S_ISDIR(m)?'d':S_ISLNK(m)?'l':S_ISCHR(m)?'c':S_ISBLK(m)?'b':S_ISFIFO(m)?'p':S_ISSOCK(m)?'s':'-',
            m&S_IRUSR?'r':'-', m&S_IWUSR?'w':'-', m&S_ISUID?(m&S_IXUSR?'s':'S'):(m&S_IXUSR?'x':'-'),
            m&S_IRGRP?'r':'-', m&S_IWGRP?'w':'-', m&S_ISGID?(m&S_IXGRP?'s':'S'):(m&S_IXGRP?'x':'-'),
            m&S_IROTH?'r':'-', m&S_IWOTH?'w':'-', m&S_ISVTX?(m&S_IXOTH?'t':'T'):(m&S_IXOTH?'x':'-'));
        
        printf("%lu ", (unsigned long)entry->st.st_nlink);

        if (flags.n) {
            printf("%u %u ", entry->st.st_uid, entry->st.st_gid);
        } else {
            struct passwd *pw = getpwuid(entry->st.st_uid);
            struct group *gr = getgrgid(entry->st.st_gid);
            printf("%s %s ", pw ? pw->pw_name : "unknown", gr ? gr->gr_name : "unknown");
        }

        if (flags.h) {
            double sz = entry->st.st_size;
            if (sz >= 1073741824) printf("%.1fG ", sz/1073741824);
            else if (sz >= 1048576) printf("%.1fM ", sz/1048576);
            else if (sz >= 1024) printf("%.1fK ", sz/1024);
            else printf("%.0fB ", sz);
        } else {
            printf("%lld ", (long long)entry->st.st_size);
        }

        char time_str[20];
        time_t t = flags.u ? entry->st.st_atime : (flags.c ? entry->st.st_ctime : entry->st.st_mtime);
        strftime(time_str, sizeof(time_str), "%b %e %H:%M", localtime(&t));
        printf("%s ", time_str);
    }
    
    printf("%s", entry->name);
    if (flags.F) {
        mode_t m = entry->st.st_mode;
        if (S_ISDIR(m)) printf("/");
        else if (S_ISLNK(m)) printf("@");
        else if (S_ISSOCK(m)) printf("=");
        else if (S_ISFIFO(m)) printf("|");
        else if (m & S_IXUSR || m & S_IXGRP || m & S_IXOTH) printf("*");
    }
    printf("\n");
}