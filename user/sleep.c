#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[]) 
{
    int i, n;

    // If the user forgets to pass an argument, sleep should print an error message.
    if(argc < 2) {
        fprintf(2, "Usage: sleep...\n");
        exit(1);
    }

    for(i = 1; i <argc; i++) {
        // string; you can convert it to an integer using atoi (see user/ulib.c).
        n = atoi(argv[i]);
        if(sleep(n) < 0) {
            fprintf(2, "sleep: %s ticks failed to pause", argv[i]);
            break;
        }
    }
    exit(0);
}