CC = clang
CFLAGS = -Wall -Wextra -Iinclude -Werror -fsanitize=address -g
LDFLAGS = -fsanitize=address
SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=build/%.o)
TARGET = build/hashmap

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

build/%.o: src/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build/*.o $(TARGET)

.PHONY: clean
