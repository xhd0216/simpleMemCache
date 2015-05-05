mtest: multitest.o cache.o
	cc -o mtest multitest.o cache.o

cache: cache.c cache.h hashmap_2.h
	cc -c cache.c
multitest: multitest.c cache.h
	cc -c multitest.c
clean: