/*
  * April 26 2021, Christian Hopps <chopps@labn.net>
  *
  * Copyright (c) 2021, LabN Consulting, L.L.C
  *
  */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "testperf.h"

extern void dosomething(const char *s);

SLIST_HEAD(nodelist, node) head = SLIST_HEAD_INITIALIZER(head);

static inline double
tdiff(struct timespec start, struct timespec end)
{
    if (end.tv_nsec < start.tv_nsec) {
        end.tv_sec--;
        end.tv_nsec += 1e9;
    }
    return (end.tv_sec - start.tv_sec) +
           (double)(end.tv_nsec - start.tv_nsec) / 1e9;
}

int
main(int argc, char **argv)
{
    struct timespec istart, iend, fstart, fend;
    double idiff, fdiff;
    struct node *n;
    long nodecount = (long)10000;
    long loopcount = (long)10000;
    long l;

    if (argc > 1)
        loopcount = strtoul(argv[1], NULL, 10);

    /* Create list of nodes */
    for (l = nodecount; l > 0; l--) {
        n = alloc(l);
        SLIST_INSERT_HEAD(&head, n, entry);
    }

    /* First time through for cache loading */
    SLIST_FOREACH(n, &head, entry) {
        const char *s = get_value_inline(n);
        //dosomething(s);
        (void)s;
    }

    clock_gettime(CLOCK_REALTIME, &istart);
    for (l = 0; l < loopcount; l++) {
        SLIST_FOREACH(n, &head, entry)
        {
            const char *s = get_value_inline(n);
            // dosomething(s);
            (void)s;
        }
    }
    clock_gettime(CLOCK_REALTIME, &iend);

    clock_gettime(CLOCK_REALTIME, &fstart);
    for (l = 0; l < loopcount; l++) {
        SLIST_FOREACH(n, &head, entry)
        {
            const char *s = get_value_func(n);
            // dosomething(s);
            (void)s;
        }
    }
    clock_gettime(CLOCK_REALTIME, &fend);

    printf("Nodecount %ld\n", nodecount);
    printf("Loopcount %ld\n", loopcount);
    idiff = tdiff(istart, iend);
    fdiff = tdiff(fstart, fend);
    printf("Inline Time %fms\n", idiff * 1000);
    printf("Non-inline Time %fms\n", fdiff * 1000);
    printf("%f%% faster\n", (fdiff - idiff)/((fdiff + idiff)/2) * 100);

    return 0;
}
