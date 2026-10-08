#ifndef OPTIONS_H
#define OPTIONS_H

typedef struct {
    int A, a, c, d, F, f, h, i, k, l, n, q, R, r, S, s, t, u, w;
} LsFlags;

extern LsFlags flags;

void parse_options(int argc, char *argv[]);

#endif