TARGET=libclib_vis.a
BUILD_DIR=build
SRC=$(wildcard *.c)
OBJ=$(patsubst %.c, %.o, $(SRC))
CC=gcc

$(TARGET): $(OBJ)
	ar -r $@ $(patsubst %.o, $(BUILD_DIR)/%.o, $^)
$(OBJ): %.o: %.c
	$(CC) -c $^ -o $(BUILD_DIR)/$@
clean:
	rm $(BUILD_DIR)/*.o $(TARGET)
