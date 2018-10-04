# Makefile for Mac
CC = g++
LIBS = -lGLEW -lSDL2 -L/usr/local/Cellar/sdl2/2.0.8/lib/ -L/usr/local/Cellar/glew/2.1.0/lib/
INCLUDE = -I/usr/local/Cellar/sdl2/2.0.8/include/SDL2/ -I/usr/local/Cellar/glew/2.1.0/include/ -Ilibs/stb_image/
FLAGS = -std=c++11 -framework OpenGL -w
OBJS = src/main.cpp src/engine/*.cpp src/engine/opengl/*.cpp src/game/*.cpp src/math/*.cpp
TARGET = -o bin/game

all: $(OBJS)
	$(CC) $(OBJS) $(LIBS) $(INCLUDE) $(FLAGS) $(TARGET)