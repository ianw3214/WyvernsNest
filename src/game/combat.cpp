#include "combat.hpp"

Combat::Combat() { }

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

	Core::Renderer::drawLine(ScreenCoord(0, Core::windowHeight() - 50), ScreenCoord(2000, Core::windowHeight() - 50), Colour(0.0, 1.0, 0.0));
	Core::Renderer::drawLine(ScreenCoord(0, Core::windowHeight() - 66), ScreenCoord(2000, Core::windowHeight() - 66), Colour(0.0, 1.0, 0.0));
	Core::Renderer::drawLine(ScreenCoord(0, Core::windowHeight() - 82), ScreenCoord(2000, Core::windowHeight() - 82), Colour(0.0, 1.0, 0.0));
	Core::Text_Renderer::setColour(Colour(0,0,0));
	Core::Text_Renderer::setAlignment(TextRenderer::hAlign::left, TextRenderer::vAlign::top);
	Core::Text_Renderer::render("Top align.", ScreenCoord(50, 50), 1.f);

	Core::Text_Renderer::setAlignment(TextRenderer::hAlign::left, TextRenderer::vAlign::bottom);
	Core::Text_Renderer::render("Bottom align.", ScreenCoord(50, 50), 1.f);

	Core::Text_Renderer::setAlignment(TextRenderer::hAlign::left, TextRenderer::vAlign::middle);
	Core::Text_Renderer::render("Middle align.", ScreenCoord(100, 50), 1.f);

	Core::Renderer::drawLine(ScreenCoord(500, 0), ScreenCoord(500, 1000), Colour(0.0, 1.0, 0.0));

	Core::Text_Renderer::setAlignment(TextRenderer::hAlign::left, TextRenderer::vAlign::top);
	Core::Text_Renderer::render("Left align.", ScreenCoord(500, 50), 1.f);

	Core::Text_Renderer::setAlignment(TextRenderer::hAlign::centre, TextRenderer::vAlign::top);
	Core::Text_Renderer::render("Centre align.", ScreenCoord(500, 80), 1.f);

	Core::Text_Renderer::setAlignment(TextRenderer::hAlign::right, TextRenderer::vAlign::top);
	Core::Text_Renderer::render("Right align.", ScreenCoord(500, 110), 1.f);

	Core::Text_Renderer::setAlignment(TextRenderer::hAlign::left, TextRenderer::vAlign::top);
}
