#pragma once

#include <queue>

#include "opengl/glwrappers.hpp"
#include "../math/vec.hpp"
#include "sprite.hpp"

// Structure to represent a queued animation
struct AnimationState {
	unsigned int animation_index;
	unsigned int loops;
};

class AnimatedSprite : public Sprite {
public:
	// Constructor of animated sprite using the width/height of each animation frame
	AnimatedSprite(const std::string& path, int frame_w, int frame_h);
	~AnimatedSprite();

	void render() override;
	void renderWithoutUpdate();
	void addAnimation(unsigned int start, unsigned int end);
	void updateFrame();

	void playAnimation(unsigned int animation, unsigned int loops = 1);
	void queueAnimation(unsigned int animation, unsigned int loops = 1);
	
	// Sprite sheet size in terms of number of sprites
	int spriteSheetWidth;
	int spriteSheetHeight;

private:

	// Animated sprite information
	std::vector<Vec2<unsigned int>> frames;

	// Animated sprite state variables
	int counter;
	unsigned int frame_index;
	unsigned int animation_index;

	// Helper function to calculate animation frame coordinates
	void updateSourcePosFromFrame();

	// Queue of upcoming animations
	std::queue<AnimationState> animations;

};