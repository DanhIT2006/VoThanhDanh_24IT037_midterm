#include "ls.h"

LsFlags flags = {0};

int main(int argc, char *argv[]) {
    int opt;
    // Phân tích các cờ đầu vào
    while ((opt = getopt(argc, argv, "AacdFfhiklnqRrSstuw")) != -1) {
        switch (opt) {
            case 'A': flags.A = 1; break;
            case 'a': flags.a = 1; break;
            case 'C': flags.C = 1; flags.u = 0; break; // -c và -u ghi đè nhau
            case 'd': flags.d = 1; flags.R = 0; break; // -d và -R ghi đè nhau[cite: 2]
            case 'F': flags.F = 1; break;
            case 'f': flags.f = 1; flags.a = 1; break; // -f không sắp xếp[cite: 1]
            case 'h': flags.h = 1; flags.k = 0; break; // -h và -k ghi đè nhau[cite: 1]
            case 'i': flags.i = 1; break;
            case 'k': flags.k = 1; flags.h = 0; break;
            case 'l': flags.l = 1; flags.n = 0; break; // -l và -n ghi đè nhau[cite: 2]
            case 'n': flags.n = 1; flags.l = 0; break;
            case 'q': flags.q = 1; flags.w = 0; break; // -q và -w ghi đè nhau[cite: 2]
            case 'R': flags.R = 1; flags.d = 0; break;
            case 'r': flags.r = 1; break;
            case 'S': flags.S = 1; break;
            case 's': flags.s = 1; break;
            case 't': flags.t = 1; break;
            case 'u': flags.u = 1; flags.C = 0; break;
            case 'w': flags.w = 1; flags.q = 0; break;
            default:
                fprintf(stderr, "Usage: %s [-AacdFfhiklnqRrSstuw] [file...]\n", argv[0]);
                exit(1);
        }
    }

    
    if (!isatty(STDOUT_FILENO) && !flags.w) {
        flags.w = 1;
    } else if (isatty(STDOUT_FILENO) && !flags.w) {
        flags.q = 1;
    }

    if (optind == argc) {
        list_directory(".");
    } else {
        for (int i = optind; i < argc; i++) {
            if (argc - optind > 1 && !flags.d) printf("%s:\n", argv[i]);
            
            if (flags.d) {
                struct stat st;
                if (lstat(argv[i], &st) == 0) {
                    print_file_info(".", argv[i], &st);
                }
            } else {
                list_directory(argv[i]);
            }
            if (i < argc - 1 && !flags.d) printf("\n");
        }
    }
    return 0;
}