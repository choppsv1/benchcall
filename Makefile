
SOCFLAGS   := -fPIC -pedantic -Wall -O2
SOLDFLAGS := -shared

CFLAGS   := -fPIC -pedantic -Wall -O2
LDFLAGS :=

all: bench

clean:
	rm *.o *.so bench

testperf.o: testperf.c testperf.h
	    gcc $(SOCFLAGS) -c testperf.c -o $@

libtestperf.so: testperf.o
	    gcc $(SOLDFLAGS) testperf.o -o $@

bench2.o: bench2.c
	    gcc $(CFLAGS) -c bench2.c -o $@
bench.o: bench.c testperf.h
	    gcc $(CFLAGS) -c bench.c -o $@

bench: bench.o bench2.o libtestperf.so
	gcc $(LDFLAGS) bench.o bench2.o -o $@ -L. -Wl,-rpath -Wl,. -ltestperf
