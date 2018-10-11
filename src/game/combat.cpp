#include "combat.hpp"

Combat::Combat(): text("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", "res/test_font.ttf", 48, Vec2<int>(Core::windowWidth(), Core::windowHeight())) {
}

Combat::~Combat() {

}

void Combat::handleEvent(const SDL_Event& e) {
	if (e.type == SDL_KEYDOWN) {
		if (e.key.keysym.sym == SDLK_SPACE) {
			// Do something when space is pressed
		}
	}
}

void Combat::update(int delta) {
	// Update shit here
}

void Combat::render() {

	// SAMPLE CODE
	Core::Renderer::drawLine(ScreenCoord(0, 0), ScreenCoord(100, 100), Colour(0.0, 1.0, 0.0));
	Sprite sprite("res/test.png");
	sprite.setPos(100, 100);
	sprite.setSize(200, 200);
	sprite.render();

	text.render(Vec3<float>(1, 1, 1), TextAlignment::center);

	Text otherText("1234567890", "res/test_font.ttf", 96, Vec2<int>(Core::windowWidth(), Core::windowHeight()));
	otherText.render(Vec3<float>(0.1f, 0.2f, 0.3f), TextAlignment::right);

	Text otherotherText("1234554321", "res/test_font.ttf", 96, Vec2<int>(Core::windowWidth(), Core::windowHeight()));
	otherotherText.render(Vec3<float>(0.8f, 0.2f, 0.2f), TextAlignment::left);
}
