#include "animatedSprite.hpp"
#include "core.hpp"

AnimatedSprite::AnimatedSprite(const std::string & path, int frame_w, int frame_h) :
	Sprite(path),
	counter(0),
	frame_index(0),
	animation_index(0)
{
	// Set the source w/h to be the same as an animation frame
	src_w = frame_w;
	src_h = frame_h;

	// Calculate the number of sprites in the sprite sheet based on the input frame size and texture size
	spriteSheetWidth = Core::getTexture(path)->getWidth() / frame_w;
	spriteSheetHeight = Core::getTexture(path)->getHeight() / frame_h;
}

AnimatedSprite::~AnimatedSprite() {

}

void AnimatedSprite::render()
{
	// Only update the frame if the timer is up
	// TODO: switch this to use a timer instead of tick counting
	if (counter < 2) {
		counter++;
	} else {
		counter = 0;
		frame_index++;
		if (frame_index > frames[animation_index].y()) {
			// Determine if we are done with the current animation state
			if (!animations.empty()) {
				animations.front().loops--;
				// If we have no loops left, remove the animation state from the sprite
				if (animations.front().loops == 0) {
					animations.pop();
				}
				// If there is still an animtion state left, use that as the new animation index
				if (!animations.empty()) animation_index = animations.front().animation_index;
			}
			frame_index = frames[animation_index].x();
		}

		// Update the sprite source to the correct animation frame
		updateSourcePosFromFrame();
	}

	// Actually draw the sprite
	Core::Renderer::drawSprite(*this);
}

void AnimatedSprite::renderWithoutUpdate() {
	// Actually draw the sprite
	Core::Renderer::drawSprite(*this);
}

void AnimatedSprite::addAnimation(unsigned int start, unsigned int end) {
	frames.push_back(Vec2<unsigned int>(start, end));
}

void AnimatedSprite::updateFrame() {
	// Only update the frame if the timer is up
	// TODO: switch this to use a timer instead of tick counting
	if (counter < 2) {
		counter++;
	} else {
		counter = 0;
		frame_index++;
		if (frame_index > frames[animation_index].y()) {
			// Determine if we are done with the current animation state
			if (!animations.empty()) {
				animations.front().loops--;
				// If we have no loops left, remove the animation state from the sprite
				if (animations.front().loops == 0) {
					animations.pop();
				}
				// If there is still an animtion state left, use that as the new animation index
				if (!animations.empty()) animation_index = animations.front().animation_index;
			}
			frame_index = frames[animation_index].x();
		}

		// Update the sprite source to the correct animation frame
		updateSourcePosFromFrame();
	}
}

void AnimatedSprite::playAnimation(unsigned int animation, unsigned int loops) {
	// Make sure the animation is valid
	// TODO: better error handling
	if (animation < 0 || animation >= frames.size()) return;
	
	// Reset the animation queue and set it to the new played animation
	while (!animations.empty()) animations.pop();
	queueAnimation(animation, loops);

	// Actually play the animation by setting it to the right frame
	animation_index = animation;
	frame_index = frames[animation_index].x();
	updateSourcePosFromFrame();
}

void AnimatedSprite::queueAnimation(unsigned int animation, unsigned int loops) {
	animations.push(AnimationState{ animation, loops });
}

// Helper function to calculate the source coordinates of the animation frame
void AnimatedSprite::updateSourcePosFromFrame() {

	int x = frame_index % spriteSheetWidth;
	int y = frame_index / spriteSheetWidth;

	// Dirty fix for inverted textures
	// TODO: Fix this in engine renderer code
	y = spriteSheetHeight - y - 1;

	setSourcePos(x * src_w, y * src_h);
}
