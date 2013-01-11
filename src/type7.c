#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "type7.h"

#define hex2int(x) (isdigit(x) ? (x) - 48 : ((x) & 0xdf) - 55)
#define int2hex(x) ((x) < 10 ? (x) + 48 : (x) + 55)

const char *xlat = "dsfd;kfoA,.iyewrkldJKDHSUBsgvca69834ncxv";

char *type7_decrypt(const char *hash)
{
    unsigned int key, i, hlen = strlen(hash) - 2;
    char *plain = malloc(hlen / 2 + 1);

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

char *type7_encrypt(const char *plain)
{
    unsigned int key, i, plen = strlen(plain);
    char *hash = malloc(plen * 2 + 3);

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
