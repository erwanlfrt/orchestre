# CFLAGS = -Wextra -Wconversion -ansi -Wpedantic -Werror -std=c11

# # TARGETS = bin/musician

# SRC = src/musician.c src/orchestra.c

# # SRC_FILES = musician.c

# BIN = bin/musician

# output: $(SRC)
# 	@gcc $< $(CFLAGS) -o $(BIN)  -pthread -lopenal -lalut
# 	@./bin/musician

musician: src/musician/musician.c
	@gcc $< src/color/colors.c src/instruments/instruments.c src/display/display.c -o bin/musician -pthread -lopenal -lalut -lncurses

orchestra: src/orchestra/orchestra.c
	@gcc $< src/instruments/instruments.c src/direction/direction.c src/audio/audio.c src/tools/tools.c src/file_reader/file_reader.c -o bin/orchestra -pthread -lopenal -lalut 

# clean:
# 	rm $(TARGETS) 
