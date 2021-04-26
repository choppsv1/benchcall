#ifndef TESTPERF_H
#define TESTPERF_H

#include <sys/queue.h>


struct node {
    SLIST_ENTRY(node) entry;
    const char *canon;
    long value;
    const char *(*print)(struct node *);
};

static inline const char *
get_value_inline(struct node *n)
{
    return n->canon ? n->canon : n->print(n);
}

extern struct node *alloc(int value);
extern const char *get_value_func(struct node *n);

#endif
