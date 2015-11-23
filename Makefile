CC=g++

pallache: pallache.cc pallache.h
	$(CC) -Wall -Wextra -std=c++14 -O3 -flto -march=native -o pallache pallache.cc

debug: pallache.cc pallache.h
	$(CC) -Wall -Wextra -std=c++14 -g3 -D PALLACHE_DEBUG -o pallache pallache.cc

clean:
	rm -f pallache
