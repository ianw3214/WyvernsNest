# Makefile for Mac
CC = g++
LIBS = -lGLEW -lSDL2 -lfreetype -L/usr/local/Cellar/sdl2/2.0.8/lib/ -L/usr/local/Cellar/glew/2.1.0/lib/ -L/usr/local/Cellar/freetype/2.9.1/lib/
INCLUDE = -I/usr/local/Cellar/sdl2/2.0.8/include/SDL2/ -I/usr/local/Cellar/glew/2.1.0/include/ -Ilibs/stb_image/ -I/usr/local/Cellar/freetype/2.9.1/include/freetype2/
FLAGS = -std=c++11 -framework OpenGL -w
OBJS = src/main.cpp src/engine/*.cpp src/engine/opengl/*.cpp src/game/*.cpp src/math/*.cpp src/engine/text/*.cpp
TARGET = -o bin/game

all: $(OBJS)
	$(CC) $(OBJS) $(LIBS) $(INCLUDE) $(FLAGS) $(TARGET)