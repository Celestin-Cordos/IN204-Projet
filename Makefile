#OBJS : files to compile
OBJS = ./src/*.cpp
#CC : compiler we're using
CC = g++

#COMPILER_FLAGS : additional compilation options
# -w suppresses all warnings
COMPILER_FLAGS = -Wall 

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = tetris.exe

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) -o $(OBJ_NAME)