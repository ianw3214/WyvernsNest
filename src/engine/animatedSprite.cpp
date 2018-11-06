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

	addAnimation(1, 1);			//idle
	addAnimation(0, 0);			//selected
	addAnimation(2, 16);		//ATK MELEE
	addAnimation(17, 31);		//ATK RANGED	
	addAnimation(32, 34);		//TAKE DMG
	addAnimation(32, 35);		//TAKE DAMAGE + DIE
	addAnimation(35, 35);		//DEAD

	animation_index = 0;
	getFramePos();
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
			//frame_index = frames[animation_index].x();
			frame_index = 0;

			ScreenCoord pos = getFramePos();
			setSourcePos(pos.x(), pos.y());

			if (animation_index == 2 || animation_index == 3 || animation_index == 4) {
				animation_index = 0;
			}

			if (animation_index == 5) {
				animation_index = 6;
				ScreenCoord pos = getFramePos();
				setSourcePos(pos.x(), pos.y());
			}

		}
		else {
			ScreenCoord pos = getFramePos();
			setSourcePos(pos.x(), pos.y());
		}
		//change the sprite

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

	y = frames_h - y - 1;

	return ScreenCoord(x * src_w, y * src_h);
}
