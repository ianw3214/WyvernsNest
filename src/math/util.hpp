#pragma once

template<typename T>
T lerp(const T& start, const T& end, float percentage) {
	return start + static_cast<T>(static_cast<float>(end - start) * percentage);
}