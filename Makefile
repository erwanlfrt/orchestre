CFLAGS = -Wextra -Wconversion -ansi -Wpedantic -Werror -std=c11

# TARGETS = bin/musician

SRC = src/musician.c src/orchestra.c

# SRC_FILES = musician.c

BIN = bin/musician

output: $(SRC)
	@gcc $< $(CFLAGS) -o $(BIN)  -pthread -lopenal -lalut
	@./bin/musician

musician: src/musician.c src/tools/colors.c
	@gcc $< $(CFLAGS) -o $(BIN)  -pthread -lopenal -lalut

orchestra: src/orchestra.c
	@gcc $< $(CFLAGS) -o $(BIN)  -pthread -lopenal -lalut
	@./bin/orchestra

# clean:
# 	rm $(TARGETS) 