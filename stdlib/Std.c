#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// definitions

extern void _WMain(void);
struct Str {
    char* buf; 
    int len;
};

// -- Public STL --

void _Wprint_n(double d)
{
    printf("%f\n", d);
}

void _Wprint_Str(struct Str* s)
{
    printf("%s\n", s->buf);
}

// -- Internal STL --

// initialize a Str object from constant char array
void Strfc(struct Str* s, char* const_char, int len)
{
    s->buf = (char*)malloc(len + 1);
    memcpy(s->buf, const_char, len);
    s->buf[len] = 0; // null byte
    s->len = len;
}

// concat two Str objects
void Strcc(struct Str* s1, struct Str* s2, struct Str* dest)
{
    dest->buf = (char*)malloc(s1->len + s2->len + 1); // allocate
    memcpy(dest->buf, s1->buf, s1->len); // copy s1
    memcpy(dest->buf + s1->len, s2->buf, s2->len); // copy s2
    dest->buf[s1->len + s2->len] = 0; // null byte
    free(s1->buf); // free s1
    free(s2->buf); // free s2
    dest->len = s1->len + s2->len;
}

int main() {
    _WMain();
    return 0;
}
