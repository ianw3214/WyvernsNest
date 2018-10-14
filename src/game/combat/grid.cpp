#include "grid.hpp"

Grid::Grid() {

}

Grid::~Grid() {

}

void Grid::update()
{
	//mouseOn = GetCursorPos(&mouse);
	SDL_GetMouseState(&mouseX, &mouseY);
}


ScreenCoord Grid::getMouseToGrid()
{
	int x = floor(mouseX / 110);
	int y = floor(mouseY / 110);

	return ScreenCoord(x,y);
}

bool Grid::isMousePosValid()
{
	return mousePos.x() < x_ && mousePos.y() < y_;
}




void Grid::render()
{
	mousePos = getMouseToGrid();

	for (int y = 0; y < y_; y++) {
		for (int x = 0; x < x_; x++) {
			if (array[y][x] == 1) {

				Sprite sprite("res/test.png");
				sprite.setPos(100 * x + (x+1) * spacing, 620 - 100 * y - (y + 1) * spacing);
				sprite.setSize(width, height);
				sprite.render();
			}
			else {
				Sprite sprite("res/test2.png");
				sprite.setPos(100 * x + (x + 1) * spacing, 620 - 100 * y - (y + 1) * spacing);
				sprite.setSize(width, height);
				sprite.render();
			}

			//selected square
			if (y == mousePos.y() && x == mousePos.x()) {
				Sprite sprite("res/test3.png");
				sprite.setPos(100 * x + (x + 1) * spacing, 620 - 100 * y - (y + 1) * spacing);
				sprite.setSize(width, height);
				sprite.render();
			}
		}
	}
}

