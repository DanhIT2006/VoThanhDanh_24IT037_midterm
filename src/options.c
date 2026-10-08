#include "options.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

LsFlags flags = {0};

void parse_options(int argc, char *argv[]) {
    int opt;
    while ((opt = getopt(argc, argv, "AacdFfhiklnqRrSstuw")) != -1) {
        switch (opt) {
            case 'A': flags.A = 1; break;
            case 'a': flags.a = 1; break;
            case 'c': flags.c = 1; flags.u = 0; break;
            case 'd': flags.d = 1; flags.R = 0; break;
            case 'F': flags.F = 1; break;
            case 'f': flags.f = 1; flags.a = 1; break;
            case 'h': flags.h = 1; flags.k = 0; break;
            case 'i': flags.i = 1; break;
            case 'k': flags.k = 1; flags.h = 0; break;
            case 'l': flags.l = 1; flags.n = 0; break;
            case 'n': flags.n = 1; flags.l = 0; break;
            case 'q': flags.q = 1; flags.w = 0; break;
            case 'R': flags.R = 1; flags.d = 0; break;
            case 'r': flags.r = 1; break;
            case 'S': flags.S = 1; break;
            case 's': flags.s = 1; break;
            case 't': flags.t = 1; break;
            case 'u': flags.u = 1; flags.c = 0; break;
            case 'w': flags.w = 1; flags.q = 0; break;
            default:
                fprintf(stderr, "Usage: %s [-AacdFfhiklnqRrSstuw] [file...]\n", argv[0]);
                exit(1);
        }
    }
    
    if (!isatty(STDOUT_FILENO) && !flags.w) flags.w = 1;
    else if (isatty(STDOUT_FILENO) && !flags.w) flags.q = 1;
}