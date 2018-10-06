#pragma once

// Templated vec2 data type
template<typename T>
struct Vec2 {

	T& x() { return m_values[0]; }
	T& y() { return m_values[1]; }

	T& u() { return m_values[0]; }
	T& v() { return m_values[1]; }

	T operator [] (unsigned i) const { return this->m_values[i]; }
	T & operator [] (unsigned i) { return this->m_values[i]; }
	operator T*() const { return this->m_values; }

    T dot(Vec2 v);
    T norm();
    Vec2 normalized();

	// Constructors
	Vec2() {}
	Vec2(T v1, T v2) {
		m_values[0] = v1;
		m_values[1] = v2;
	}

private:
	T m_values[2];
};

// Templated vec3 data type
template<typename T>
struct Vec3 {

	T& x() { return m_values[0]; }
	T& y() { return m_values[1]; }
	T& z() { return m_values[2]; }

	T& u() { return m_values[0]; }
	T& v() { return m_values[1]; }
	T& w() { return m_values[2]; }

	T& r() { return m_values[0]; }
	T& g() { return m_values[1]; }
	T& b() { return m_values[2]; }

	T operator [] (unsigned i) const { return this->m_values[i]; }
	T & operator [] (unsigned i) { return this->m_values[i]; }
	operator T*() const { return this->m_values; }

    T dot(Vec3 v);
    T norm();
    Vec3 normalized();

	// Constructors
	Vec3() {}
	Vec3(T v1, T v2, T v3) {
		m_values[0] = v1;
		m_values[1] = v2;
		m_values[2] = v3;
	}

private:
	T m_values[3];

};

// Common aliases for vector types
using Colour = Vec3<float>;
using ScreenCoord = Vec2<int>;
