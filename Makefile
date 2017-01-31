CC=g++
TATTOO=-D TATTOO='"$(shell git rev-parse --verify HEAD)"'
SHASUM=-D SHASUM='"$(shell cat pallache.h pallache.cc | sha1sum | cut -d' ' -f 1)"'

pallache: pallache.cc pallache.h
	$(CC) $(TATTOO) $(SHASUM) -Wall -Wextra -std=c++14 -O3 -flto -march=native -o pallache pallache.cc

debug: pallache.cc pallache.h
	$(CC) $(TATTOO) $(SHASUM) -Wall -Wextra -std=c++14 -g3 -D PALLACHE_DEBUG -o pallache pallache.cc

test: pallache
	./regtest

clean:
	rm -f pallache
