#pragma once

#include "../engine/core.hpp"

// NOTE: These functions do the same things, but fuck it
namespace SubDiv {

	inline int hPos(int divisors, int num) {
		return (Core::windowWidth() / divisors) * num;
	}

	inline int vPos(int divisors, int num) {
		return (Core::windowHeight() / divisors) * num;
	}

	inline int hSize(int divisors, int blocks) {
		return (Core::windowWidth() / divisors) * blocks;
	}

	inline int vSize(int divisors, int blocks) {
		return (Core::windowHeight() / divisors) * blocks;
	}

	inline int hCenter() {
		return Core::windowWidth() / 2;
	}

	inline int vCenter() {
		return Core::windowHeight() / 2;
	}

}