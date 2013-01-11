#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>


#define hex2int(x) (isdigit(x) ? (x) - 48 : ((x) & 0xdf) - 55)
#define int2hex(x) ((x) < 10 ? (x) + 48 : (x) + 55)


const char *xlat = "dsfd;kfoA,.iyewrkldJKDHSUBsgvca69834ncxv";


void usage(void);

char *unseven(const char*);
char *enseven(const char*);


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
        char *s = encrypt ? enseven(buf) : unseven(buf);

        if (s == NULL)
            fprintf(stderr, "! Error: %s\n", buf);
        else {
            printf("%s\t%s\n", buf, s);
            free(s);
        }
    }

    return 0;
}


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


char *unseven(const char *hash)
{
    unsigned int key, i, hlen = strlen(hash) - 2;
    char *plain = (char*)malloc(hlen / 2 + 1);

    if (hlen < 2 || hlen & 1) return NULL;

    key = (hash[0] - '0') * 10 + hash[1] - '0';
    if (key > 15 || !isdigit(hash[0]) || !isdigit(hash[1])) return NULL;

    hash += 2;
    for (i = 0; i < hlen; ++i) if (!isxdigit(hash[i])) return NULL;

    for (i = 0; i < hlen; i += 2) {
        plain[i / 2] = ((hex2int(hash[i]) << 4) | hex2int(hash[i + 1])) ^ xlat[key++];
        if (key == 40) key = 0;
    }
    plain[hlen / 2] = 0;

    return plain;
}

char *enseven(const char *plain)
{
    unsigned int key, i, plen = strlen(plain);
    char *hash = (char*)malloc(plen * 2 + 3);

    if (plen > 25) plen = 25;
    key = rand() % 16;

    hash[0] = key > 9 ? '1' : '0';
    hash[1] = key % 10 + '0';

    for (i = 2; i <= plen * 2; ++i) {
        char c = plain[i / 2 - 1] ^ xlat[key++];

        hash[i]   = int2hex(c >> 4);
        hash[++i] = int2hex(c & 15);
    }
    hash[i] = 0;

    return hash;
}
