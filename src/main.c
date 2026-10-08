#include "options.h"
#include "traverse.h"

int main(int argc, char *argv[]) {
    parse_options(argc, argv);
    handle_arguments(argc, argv);
    return 0;
}