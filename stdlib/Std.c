#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "gc/tgc.h"

// definition
extern void _Wmain(void);
struct Str {
    char* buf; 
    int len;
};

tgc_t GC;

// -- Public STL --

void _Wprint_n(double d)
{
    char buf[20];
    snprintf(buf, 20, "%.7f", d);
    for (int i = strlen(buf) - 1; i > 0; --i)
    {
        if (buf[i] == '0')
            buf[i] = 0; // remove the tailing zeros by terminating the string
        else break;
    }
    // if the number is an integer, remove the tailing dot   
    if (buf[strlen(buf) - 1] == '.') buf[strlen(buf) - 1] = '\0';
    puts(buf);
}

void _Wprint_Str(struct Str* s)
{
    printf("%s\n", s->buf);
}

// -- Internal STL --

// initialize a Str object from constant char array

void* alloc(int size)
{
    return tgc_alloc_opt(&GC, (size_t)size, 0, NULL); // is equal to tgc_alloc, just removes one unnecessary function call
}

struct Str* strfc(char* const_char, int len)
{
    struct Str* s = tgc_alloc(&GC, sizeof(struct Str));
    s->buf = const_char;
    s->len = len;
}

// concat two Str objects
struct Str* strcc(struct Str* s1, struct Str* s2)
{
    struct Str* dest = tgc_alloc(&GC, sizeof(struct Str));
    dest->buf = tgc_alloc(&GC, s1->len + s2->len + 1);
    memcpy(dest->buf, s1->buf, s1->len); // copy s1
    memcpy(dest->buf + s1->len, s2->buf, s2->len); // copy s2
    dest->buf[s1->len + s2->len] = 0; // null byte
    dest->len = s1->len + s2->len;
    return dest;
}

int main(int argc, char** argv) {
    tgc_start(&GC, &argc);
    _Wmain();
    tgc_stop(&GC);
    return 0;
}
