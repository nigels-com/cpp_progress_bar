CC = g++
CPPFLAGS = -std=c++17
TARGET = progress_bar
OBJ = main.o

all : progress_bar

progress_bar : $(OBJ)
	@echo "<***Linking***> $@"
	@$(CC) $(CPPFLAGS) $(OBJ) -o $(TARGET)

main.o : main.cpp progress_bar.h
	@echo "<**Compiling**> $@"
	@$(CC) $(CPPFLAGS) -c $^

clean :
	@rm -rf progress_bar $(OBJ)
