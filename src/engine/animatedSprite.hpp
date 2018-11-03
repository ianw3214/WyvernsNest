#pragma once
#include "opengl/glwrappers.hpp"
#include "../math/vec.hpp"
#include "sprite.hpp"


class AnimatedSprite : public Sprite {
public:
	AnimatedSprite(const std::string & path, int w, int h);
	~AnimatedSprite();

	void render();


	int w, h;
	int src_w, src_h;
	int original_w, original_h;

	std::vector<ScreenCoord> frames;
	void addAnimation(int start, int end);

	ScreenCoord getFramePos();

	int counter;
	int frame_index;
	int animation_index;

	int frames_w;
	int frames_h;

};