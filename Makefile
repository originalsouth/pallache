CC=g++
TATTOO_VAL=$(shell git rev-parse --verify HEAD)
SHASUM_VAL=$(shell cat pallache.h pallache.cc | sha1sum | cut -d' ' -f 1)
TATTOO=-D TATTOO='"$(TATTOO_VAL)"'
SHASUM=-D SHASUM='"$(SHASUM_VAL)"'

pallache: pallache.cc pallache.h
	$(CC) $(TATTOO) $(SHASUM) -Wall -Wextra -std=c++14 -O3 -flto -march=native -o pallache pallache.cc

debug: pallache.cc pallache.h
	$(CC) $(TATTOO) $(SHASUM) -Wall -Wextra -std=c++14 -g3 -D PALLACHE_DEBUG -o pallache pallache.cc

test: pallache
	@./regtest

version:
	@printf "pallache-make: git-hash\t%s\n" $(TATTOO_VAL)
	@printf "pallache-make: git-hash\t%s\n" $(SHASUM_VAL)

clean:
	rm -f pallache
