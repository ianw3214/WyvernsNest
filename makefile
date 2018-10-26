# Makefile for macOS
CXX = g++
LIBS = -lGLEW -lSDL2 -lfreetype -L/usr/local/Cellar/sdl2/2.0.8/lib/ -L/usr/local/Cellar/glew/2.1.0/lib/ -L/usr/local/Cellar/freetype/2.9.1/lib/
INCLUDE = -I/usr/local/Cellar/sdl2/2.0.8/include/SDL2/ -I/usr/local/Cellar/glew/2.1.0/include/ -Ilibs/stb_image/ -I/usr/local/Cellar/freetype/2.9.1/include/freetype2/
LFLAGS = -std=c++11 -framework OpenGL -w $(LIBS) $(INCLUDE)
CFLAGS = $(LFLAGS) -o bin/objs/$@
OBJS = engine.o entity.o indexBuffer.o shader.o texture.o vertexArray.o vertexBuffer.o renderer.o sprite.o state.o combat.o customization.o cutscene.o vec.o
BIN_OBJS = $(addprefix bin/objs/, $(OBJS))

VPATH = src src/engine src/engine/opengl src/engine/text src/game src/math bin bin/objs
TARGET = bin/game

all: $(TARGET)

$(TARGET): bin bin_objs $(OBJS) main.cpp
	$(CXX) $(LFLAGS) $(BIN_OBJS) src/main.cpp -o $(TARGET)

run: $(TARGET)
	./bin/game

bin:
	mkdir -p bin

bin_objs:
	mkdir -p bin/objs	

engine.o: engine.cpp renderer.hpp state.hpp
entity.o: entity.cpp
indexBuffer.o: indexBuffer.cpp
shader.o: shader.cpp
texture.o: texture.cpp
vertexArray.o: vertexArray.cpp vertexBuffer.hpp vertexBufferLayout.hpp
vertexBuffer.o: vertexBuffer.cpp
renderer.o: renderer.cpp glwrappers.hpp math.hpp
sprite.o: sprite.cpp glwrappers.hpp
state.o: state.cpp
combat.o: combat.cpp core.hpp
customization.o: customization.cpp core.hpp unitData.hpp
cutscene.o: cutscene.cpp core.hpp
vec.o: vec.cpp

.cpp.o:
	$(CXX) $(CFLAGS) -c $<

clean:
	@\rm -rf bin/objs/*.o $(TARGET)