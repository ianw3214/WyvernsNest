#pragma once

#include "../../engine/core.hpp"
#include "Windows.h"
#include "../../math/vec.hpp"
#include <math.h> 


class Grid {	

public:

	Grid();
	~Grid();

	void render();
	void update();
	ScreenCoord getMouseToGrid();

	bool isMousePosValid();

	int array[3][4] = {
		{1, 1, 0, 0} ,   
		{1, 1, 1, 1} ,   
		{0, 1, 0, 0}   
	};

	int y_ = 3;
	int x_ = 4;

	int width = 100;
	int height = 100;

	int spacing = 10;


	int mouseX;
	int mouseY;

	ScreenCoord mousePos;

private:
	Sprite *m_test1;
	Sprite *m_test2;
	Sprite *m_test3;

};