TARGET=libclib_ds.a
BUILD_DIR=build
SRC=$(wildcard *.c)
OBJ=$(patsubst %.c, %.o, $(SRC))
CC=gcc

$(TARGET): $(OBJ)
	echo $(OBJ)
	ar -r $@ $(patsubst %.o, $(BUILD_DIR)/%.o, $^)
$(OBJ): %.o: %.c
	$(CC) -c $^ -o $(BUILD_DIR)/$@
clean:
	rm $(BUILD_DIR)/*.o $(BUILD_DIR)/$(TARGET)
