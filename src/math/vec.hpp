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

    T dot(Vec2 v) {
        x = this->m_values[0] * v->m_values[0];
        y = this->m_values[1] * v->m_values[1];

        return x + y;
    }
    T norm() {
        T x = pow(this->m_values[0], 2);
        T y = pow(this->m_values[1], 2);

        return sqrt(x + y);
    }
    Vec2 normalized() {
        x = this->m_values[0] * abs(this->m_values[0]);
        y = this->m_values[1] * abs(this->m_values[1]);
        Vec2 n(x, y);

        return n;
    }

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

    T dot(Vec3 v) {
        T x = this->m_values[0] * v->m_values[0];
        T y = this->m_values[1] * v->m_values[1];
        T z = this->m_values[2] * v->m_values[2];

        return x + y + z;
    }
    T norm() {
        T x = pow(this->m_values[0], 2);
        T y = pow(this->m_values[1], 2);
        T z = pow(this->m_values[2], 2);
            
        return sqrt(x + y + z);
    }
    Vec3 normalized() {
        T x = this->m_values[0] * abs(this->m_values[0]);
        T y = this->m_values[1] * abs(this->m_values[1]);
        T z = this->m_values[2] * abs(this->m_values[2]);
        Vec3 n(x, y, z);

        return n;
    }

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
