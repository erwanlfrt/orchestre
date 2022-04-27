CFLAGS = -Wextra -Wconversion -ansi -Wpedantic -Werror -std=c11

# TARGETS = bin/musician

SRC = src/musician.c src/orchestra.c

# SRC_FILES = musician.c

BIN = bin/musician

output: $(SRC)
	@gcc $< $(CFLAGS) -o $(BIN)  -pthread -lopenal -lalut
	@./bin/musician

musician: src/musician.c src/color/colors.c
	@gcc $< $(CFLAGS) -o $(BIN)  -pthread -lopenal -lalut

orchestra: src/orchestra.c
	@gcc $< $(CFLAGS) -o $(BIN)  -pthread -lopenal -lalut
	@./bin/orchestra

# clean:
# 	rm $(TARGETS) 

gcc src/orchestra/orchestra.c src/instruments/instruments.c src/direction/direction.c src/audio/audio.c src/tools/tools.c src/file_reader/file_reader.c -o bin/orchestra -pthread -lopenal -lalut 



&& gcc src/musician/musician.c src/color/colors.c src/instruments/instruments.c -o bin/musician -pthread -lopenal -lalut -lncurses