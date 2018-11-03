#include "animatedSprite.hpp"
#include "core.hpp"

AnimatedSprite::AnimatedSprite(const std::string & path, int w, int h) :
	Sprite(path),
	counter(0)
{
	src_w = original_w  = w;
	src_h = original_h  = h;

	frames_w = Core::getTexture(path)->getWidth() / w;
	frames_h = Core::getTexture(path)->getHeight() / h;

	addAnimation(0, 6);
	animation_index = 0;
}

AnimatedSprite::~AnimatedSprite()
{
}

void AnimatedSprite::render()
{
	if (counter < 2) {
		counter++;
	}
	else {
		counter = 0;
		frame_index++;
		if (frame_index > frames[animation_index].y() - frames[animation_index].x()) {
			frame_index = frames[animation_index].x();
		}
		//change the sprite
		ScreenCoord pos = getFramePos();
		setSourcePos(pos.x(),pos.y());
	}

	Core::Renderer::drawSprite(*this);
}

void AnimatedSprite::addAnimation(int start, int end)
{
	frames.push_back(ScreenCoord(start, end));
}

ScreenCoord AnimatedSprite::getFramePos()
{
	int first = frames[animation_index].x();
	int x = (first + frame_index) % frames_w;
	int y = (first + frame_index) / frames_w;
	//int y = 0;



	return ScreenCoord(x * src_w, y * src_h);
}
