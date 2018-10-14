#include "grid.hpp"

Grid::Grid() {
	m_test1 = ResourceEngine::getSprite("test1");
	m_test2 = ResourceEngine::getSprite("test2");
	m_test3 = ResourceEngine::getSprite("test3");
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
	int x = static_cast<int>(floor(mouseX / 110));
	int y = static_cast<int>(floor(mouseY / 110));

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

				m_test1->render(100 * x + (x+1) * spacing, 620 - 100 * y - (y + 1) * spacing, width, height);
			}
			else {
				m_test2->render(100 * x + (x + 1) * spacing, 620 - 100 * y - (y + 1) * spacing, width, height);
			}

			//selected square
			if (y == mousePos.y() && x == mousePos.x()) {
				m_test3->render(100 * x + (x + 1) * spacing, 620 - 100 * y - (y + 1) * spacing, width, height);
			}
		}
	}
}

