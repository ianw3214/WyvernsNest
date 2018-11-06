#define SDL_MAIN_HANDLED

#include <iostream>

#include "engine/core.hpp"
#include "game/menu.hpp"

int main(int argc, char* argv[]) {

	// Initialize the engine
	if (!Core::init("Wyverns Nest", 1280, 720)) {
		std::cerr << "Initialization failed\n";
		return 1;
	}

	// Core::setDebugMode(true);

	// Combat * state = new Combat();
	Menu * state = new Menu();
	Core::setState(state);

	/*
	//	---------------TEMPORARY AUDIO CODE--------------------------
	// load WAV file
	SDL_AudioSpec wavSpec;
	Uint32 wavLength;
	Uint8 *wavBuffer;
	if (SDL_LoadWAV("res/music/track1.wav", &wavSpec, &wavBuffer, &wavLength) == NULL) {
		return 1;
	}
	// open audio device
	SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
	// play audio
	int success = SDL_QueueAudio(deviceId, wavBuffer, wavLength);
	SDL_PauseAudioDevice(deviceId, 0);
	//	-------------------------------------------------------------
	*/

	// Main engine loop
	while (Core::running()) {
		Core::update();
	}

	return 0;

}