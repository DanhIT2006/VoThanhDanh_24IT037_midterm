#ifndef DISPLAY_H
#define DISPLAY_H

#include "file_entry.h"

void print_file(FileEntry *entry);
void print_total_blocks(FileEntry *entries, int count);

#endif