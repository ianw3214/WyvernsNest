#include "pausemenu.hpp"

#include "util/util.hpp"
#include "menus/menu.hpp"

PauseMenu::PauseMenu() : 
	pauseSprite("res/assets/UI/pauseBase.png")
{
	initSprites();
}


PauseMenu::~PauseMenu()
{
}

void PauseMenu::initSprites() {
	// Compute pause menu sprite position
	int width = pauseSprite.getTexture().getWidth();
	int height = pauseSprite.getTexture().getHeight();
	pauseSprite.setPos((Core::windowWidth() - width) / 2, (Core::windowHeight() - height) / 2);

	// init menu buttons
	continueButton = ButtonData(ScreenCoord(SubDiv::hCenter() - 120, SubDiv::vPos(40, 27)));
	resumeButton = ButtonData(ScreenCoord(SubDiv::hCenter() - 120, SubDiv::vCenter() - 100), 240);
	menuButton = ButtonData(ScreenCoord(SubDiv::hCenter() - 120, SubDiv::vCenter()), 240);
	quitButton = ButtonData(ScreenCoord(SubDiv::hCenter() - 120, SubDiv::vCenter() + 100), 240);
}

void PauseMenu::handleEvent(const SDL_Event& e) {
	if (e.type == SDL_MOUSEBUTTONUP) {
		ScreenCoord mouse;
		SDL_GetMouseState(&mouse.x(), &mouse.y());
		if (resumeButton.colliding(mouse)) {
			isShown = false;
		}
		if (menuButton.colliding(mouse)) {
			changeState(new Menu());
		}
		if (quitButton.colliding(mouse)) {
			exit(0);
		}
	}
}

void PauseMenu::update(int delta) {
}

void PauseMenu::render() {
	if (isShown) {
		pauseSprite.render();
		Core::Text_Renderer::setAlignment(TextRenderer::hAlign::centre, TextRenderer::vAlign::bottom);
		Core::Text_Renderer::setColour(Colour(.4f, .2f, .1f));
		Core::Text_Renderer::render("PAUSE", ScreenCoord(SubDiv::hCenter(), SubDiv::vPos(10, 3)), 2.5f);
		Core::Text_Renderer::setColour(Colour(0.f, 0.f, 0.f));
		Core::Text_Renderer::setAlignment(TextRenderer::hAlign::centre, TextRenderer::vAlign::middle);
		resumeButton.render();
		Core::Text_Renderer::render("RESUME", resumeButton.position + Vec2<int>(120, 32), 1.8f);
		menuButton.render();
		Core::Text_Renderer::render("MAIN MENU", menuButton.position + Vec2<int>(120, 32), 1.8f);
		quitButton.render();
		Core::Text_Renderer::render("QUIT", quitButton.position + Vec2<int>(120, 32), 1.8f);
	}
}

void PauseMenu::setIsShown(bool value) {
	isShown = value;
}

bool PauseMenu::getIsShown() {
	return isShown;
}