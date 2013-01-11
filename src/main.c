#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "type7.h"

void usage(void)
{
    printf("\033[1mUSAGE\033[0m\n"
           "\t\033[1menseven\033[0m \033[4mPLAINTEXT\033[0m...\n"
           "\t\033[1munseven\033[0m \033[4mHASH\033[0m...\n"
           "\n"
           "\033[1mSYNOPSIS\033[0m\n"
           "\tEncrypts or decrypts passwords using Cisco's type 7 "
           "encryption. As\n\texpected, \033[1menseven\033[0m encrypts "
           "and \033[1munseven\033[0m decrypts.\n"
           "\tIt can also read them from stdin, one per line.\n\n");
    exit(0);
}

char *next(int argc, char **argv)
{
    static char buf[54];

    if (argv == NULL) {
        fgets(buf, 54, stdin);
        
        if (!*buf)
            return NULL;
            
        if (buf[strlen(buf) - 1] != '\n') {
            char c;
            while ((c = getchar()) != '\n' && c != EOF);
        }

        return buf;
    } else {
        static int ind = 0;

        return ++ind < argc ? argv[ind] : NULL;
    }
}

int main(int argc, char **argv)
{
    int from_stdin, encrypt;
    char *buf;

    if ((encrypt = strstr(argv[0], "enseven") != NULL))
        srand((unsigned int)time(NULL));

    from_stdin = !isatty(fileno(stdin));
    if (!from_stdin && argc < 2) usage();

    while ((buf = next(argc, from_stdin ? NULL : argv))) {
        char *s = encrypt ? type7_encrypt(buf) : type7_decrypt(buf);

        if (s == NULL)
            fprintf(stderr, "! Error: %s\n", buf);
        else {
            printf("%s\t%s\n", buf, s);
            free(s);
        }
    }

    return 0;
}
