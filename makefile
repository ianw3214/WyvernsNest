# Makefile for macOS - Automatically generated
CXX = g++
LIBS = -lGLEW -lSDL2 -lSDL2_mixer -lfreetype -L/usr/local/Cellar/sdl2/2.0.9/lib/ -L/usr/local/Cellar/glew/2.1.0/lib/ -L/usr/local/Cellar/freetype/2.9.1/lib/ -L/usr/local/Cellar/sdl2_mixer/2.0.4/lib
INCLUDE = -I/usr/local/Cellar/sdl2/2.0.9/include/SDL2/ -Isrc/vendor -I/usr/local/Cellar/glew/2.1.0/include/ -Ilibs/stb_image/ -I/usr/local/Cellar/freetype/2.9.1/include/freetype2/ -I/usr/local/Cellar/sdl2_mixer/2.0.4/include/SDL2
LFLAGS = -std=c++11 -framework OpenGL -w $(LIBS) $(INCLUDE) -D_DEBUG
CFLAGS = $(LFLAGS) -o bin/objs/$@
OBJS = skillTree.o combat.o customization.o player.o status.o attack.o grid.o unit.o enemy.o basicWarriorEnemy.o mageDudeEnemy.o button.o particleloader.o attackloader.o creditsmenu.o cutscene.o menu.o settingsmenu.o vec.o entity.o particleSystem.o sprite.o state.o mixer.o animatedSprite.o textureManager.o engine.o renderer.o font.o textRenderer.o vertexBuffer.o vertexArray.o texture.o shader.o indexBuffer.o
BIN_OBJS = $(addprefix bin/objs/, $(OBJS))
IMPL_FILES = main.cpp skillTree.cpp combat.cpp customization.cpp player.cpp status.cpp attack.cpp grid.cpp unit.cpp enemy.cpp basicWarriorEnemy.cpp mageDudeEnemy.cpp button.cpp particleloader.cpp attackloader.cpp creditsmenu.cpp cutscene.cpp menu.cpp settingsmenu.cpp vec.cpp entity.cpp particleSystem.cpp sprite.cpp state.cpp mixer.cpp animatedSprite.cpp textureManager.cpp engine.cpp renderer.cpp font.cpp textRenderer.cpp vertexBuffer.cpp vertexArray.cpp texture.cpp shader.cpp indexBuffer.cpp
HEADER_FILES = skillTree.hpp combat.hpp customization.hpp player.hpp status.hpp attack.hpp grid.hpp unit.hpp enemy.hpp basicWarriorEnemy.hpp mageDudeEnemy.hpp button.hpp particleloader.hpp attackloader.hpp creditsmenu.hpp cutscene.hpp menu.hpp settingsmenu.hpp vec.hpp entity.hpp particleSystem.hpp sprite.hpp state.hpp mixer.hpp animatedSprite.hpp textureManager.hpp engine.hpp renderer.hpp font.hpp textRenderer.hpp vertexBuffer.hpp vertexArray.hpp texture.hpp shader.hpp indexBuffer.hpp
VPATH = libs libs/SDL2-2.0.9 libs/SDL2-2.0.8/include libs/SDL2-2.0.8/docs libs/SDL2-2.0.8/lib libs/SDL2-2.0.8/lib/x64 libs/SDL2-2.0.8/lib/x86 libs/stb_image libs/glew-2.1.0 libs/glew-2.1.0/bin libs/glew-2.1.0/bin/Release libs/glew-2.1.0/bin/Release/x64 libs/glew-2.1.0/bin/Release/Win32 libs/glew-2.1.0/include libs/glew-2.1.0/include/GL libs/glew-2.1.0/lib libs/glew-2.1.0/lib/Release libs/glew-2.1.0/lib/Release/x64 libs/glew-2.1.0/lib/Release/Win32 libs/glew-2.1.0/doc bin/objs bin bin/objs src src/game src/game/combat src/game/combat/enemies src/game/util src/game/menus src/math src/engine src/engine/text src/engine/opengl src/vendor src/vendor/nlohmann
TARGET = bin/game

all: $(TARGET)

$(TARGET): bin bin/objs $(OBJS) $(IMPL_FILES) $(HEADER_FILES)
	$(CXX) $(LFLAGS) $(BIN_OBJS) src/main.cpp -o $(TARGET)

run: $(TARGET)
	./bin/game

bin:
	mkdir -p bin

bin/objs:
	mkdir -p bin/objs

skillTree.o: skillTree.cpp  skillTree.hpp customization.hpp core.hpp unitData.hpp button.hpp
combat.o: combat.cpp  combat.hpp util.hpp attackloader.hpp particleloader.hpp unit.hpp player.hpp enemy.hpp status.hpp mageDudeEnemy.hpp basicWarriorEnemy.hpp menu.hpp creditsmenu.hpp customization.hpp core.hpp vec.hpp grid.hpp particleSystem.hpp button.hpp unitData.hpp
customization.o: customization.cpp  customization.hpp skillTree.hpp combat.hpp util.hpp core.hpp button.hpp unitData.hpp
player.o: player.cpp  player.hpp combat.hpp attackloader.hpp util.hpp core.hpp vec.hpp animatedSprite.hpp attack.hpp unit.hpp
status.o: status.cpp  status.hpp unit.hpp unitData.hpp
attack.o: attack.cpp  attack.hpp combat.hpp unit.hpp status.hpp particleloader.hpp core.hpp vec.hpp attackEffects.hpp
grid.o: grid.cpp  grid.hpp core.hpp vec.hpp
unit.o: unit.cpp  unit.hpp attackloader.hpp combat.hpp core.hpp attack.hpp status.hpp unitData.hpp
enemy.o: enemy.cpp  enemy.hpp combat.hpp attackloader.hpp core.hpp vec.hpp animatedSprite.hpp unit.hpp attack.hpp
basicWarriorEnemy.o: basicWarriorEnemy.cpp  basicWarriorEnemy.hpp combat.hpp attackloader.hpp player.hpp enemy.hpp
mageDudeEnemy.o: mageDudeEnemy.cpp  mageDudeEnemy.hpp combat.hpp attackloader.hpp enemy.hpp
button.o: button.cpp  button.hpp core.hpp
particleloader.o: particleloader.cpp  particleloader.hpp particleSystem.hpp
attackloader.o: attackloader.cpp  attackloader.hpp attack.hpp
creditsmenu.o: creditsmenu.cpp  creditsmenu.hpp menu.hpp core.hpp
cutscene.o: cutscene.cpp  cutscene.hpp core.hpp
menu.o: menu.cpp  menu.hpp util.hpp settingsmenu.hpp creditsmenu.hpp combat.hpp cutscene.hpp core.hpp
settingsmenu.o: settingsmenu.cpp  settingsmenu.hpp menu.hpp util.hpp core.hpp
vec.o: vec.cpp  vec.hpp
entity.o: entity.cpp  entity.hpp
particleSystem.o: particleSystem.cpp  particleSystem.hpp core.hpp
sprite.o: sprite.cpp  sprite.hpp core.hpp glwrappers.hpp
state.o: state.cpp  state.hpp engine.hpp entity.hpp
mixer.o: mixer.cpp  mixer.hpp
animatedSprite.o: animatedSprite.cpp  animatedSprite.hpp core.hpp glwrappers.hpp vec.hpp sprite.hpp
textureManager.o: textureManager.cpp  textureManager.hpp texture.hpp
engine.o: engine.cpp  engine.hpp renderer.hpp textRenderer.hpp textureManager.hpp state.hpp mixer.hpp
renderer.o: renderer.cpp  renderer.hpp engine.hpp sprite.hpp glwrappers.hpp math.hpp
font.o: font.cpp  font.hpp vec.hpp
textRenderer.o: textRenderer.cpp  textRenderer.hpp font.hpp glwrappers.hpp
vertexBuffer.o: vertexBuffer.cpp  vertexBuffer.hpp
vertexArray.o: vertexArray.cpp  vertexArray.hpp vertexBuffer.hpp vertexBufferLayout.hpp
texture.o: texture.cpp  texture.hpp stb_image.h
shader.o: shader.cpp  shader.hpp
indexBuffer.o: indexBuffer.cpp  indexBuffer.hpp

.cpp.o:
	$(CXX) $(CFLAGS) -c $<

clean:
	@\rm -rf bin