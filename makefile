# Makefile for macOS
CXX = g++
LIBS = -lGLEW -lSDL2 -lSDL2_mixer -lfreetype -L/usr/local/Cellar/sdl2/2.0.8/lib/ -L/usr/local/Cellar/glew/2.1.0/lib/ -L/usr/local/Cellar/freetype/2.9.1/lib/ -L/usr/local/Cellar/sdl2_mixer/2.0.4/lib
INCLUDE = -Isrc/vendor -I/usr/local/Cellar/sdl2/2.0.8/include/SDL2/ -I /usr/local/Cellar/sdl2_mixer/2.0.4/include/SDL2 -I/usr/local/Cellar/glew/2.1.0/include/ -Ilibs/stb_image/ -I/usr/local/Cellar/freetype/2.9.1/include/freetype2/
LFLAGS = -std=c++11 -framework OpenGL -w $(LIBS) $(INCLUDE) -D_DEBUG
CFLAGS = $(LFLAGS) -o bin/objs/$@
OBJS = mixer.o attackloader.o textureManager.o animatedSprite.o engine.o entity.o indexBuffer.o shader.o texture.o vertexArray.o vertexBuffer.o renderer.o sprite.o state.o combat.o customization.o skillTree.o cutscene.o vec.o textRenderer.o font.o player.o grid.o enemy.o menu.o unit.o attack.o creditsmenu.o attackEffects.o status.o button.o settingsmenu.o mageDudeEnemy.o basicWarriorEnemy.o
BIN_OBJS = $(addprefix bin/objs/, $(OBJS))
VPATH = src src/engine src/engine/opengl src/engine/text src/game src/game/combat src/game/combat/enemies src/game/menus src/math bin bin/objs src/game/util
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
combat.o: combat.cpp core.hpp mageDudeEnemy.hpp basicWarriorEnemy.hpp
customization.o: customization.cpp core.hpp unitData.hpp
skillTree.o: skillTree.cpp core.hpp unitData.hpp
cutscene.o: cutscene.cpp core.hpp
vec.o: vec.cpp
player.o: player.cpp core.hpp vec.hpp attack.hpp
grid.o: grid.cpp core.hpp vec.hpp player.hpp enemy.hpp
enemy.o: enemy.cpp core.hpp vec.hpp
menu.o: menu.cpp core.hpp
unit.o: unit.cpp core.hpp attack.hpp unitData.hpp
attack.o: attack.cpp core.hpp vec.hpp
mageDudeEnemy.o: mageDudeEnemy.cpp
basicWarriorEnemy.o: basicWarriorEnemy.cpp
attackloader.o: attackloader.cpp attack.hpp
textureManager.o: textureManager.cpp
animatedSprite.o: animatedSprite.cpp glwrappers.hpp vec.hpp sprite.hpp
mixer.o: mixer.cpp
attackEffects.o: attackEffects.cpp unitData.hpp
status.o: status.cpp unitData.hpp
creditsmenu.o: creditsmenu.cpp core.hpp
settingsmenu.o: settingsmenu.cpp core.hpp
button.o: button.cpp core.hpp
mageDudeEnemy.o: mageDudeEnemy.cpp enemy.hpp
basicWarriorEnemy.o: basicWarriorEnemy.cpp enemy.hpp

.cpp.o:
	$(CXX) $(CFLAGS) -c $<

clean:
	@\rm -rf bin/objs/*.o $(TARGET)