
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "testperf.h"

const char *
get_value_func(struct node *n)
{
    return get_value_inline(n);
}

const char *
printfunc(struct node *n)
{
    char value[20];

    if (!n->canon) {
        sprintf(value, "%ld", n->value);
        n->canon = strdup(value);
    }
    return n->canon;
}

struct node *
alloc(int value)
{
    struct node *n = malloc(sizeof(*n));
    memset(n, 0, sizeof(*n));
    n->value = value;
    n->print = printfunc;
    return n;
}
