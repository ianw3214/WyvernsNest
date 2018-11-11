#pragma once
#include <cmath>

// Templated vec2 data type
template<typename T>
struct Vec2 {

	T& x() { return m_values[0]; }
	T& y() { return m_values[1]; }

	T& u() { return m_values[0]; }
	T& v() { return m_values[1]; }

	// Constructors
	Vec2() {}
	Vec2(T v1, T v2) {
		m_values[0] = v1;
		m_values[1] = v2;
	}

	// Operators
	T operator [] (unsigned i) const { return this->m_values[i]; }
	T& operator [] (unsigned i) { return this->m_values[i]; }
	T const * operator *() const { return this->m_values; } // Used to pass raw data

	Vec2<T>& operator*=(const T& rhs) {
		(*this)[0] *= rhs;
		(*this)[1] *= rhs;
		return *this;
	}

	Vec2<T> operator*(const T& rhs) {
		Vec2<T> out(*this);
		out *= rhs;
		return out;
	}

	Vec2<T>& operator/=(const T& rhs) {
		(*this)[0] /= rhs;
		(*this)[1] /= rhs;
		return *this;
	}

	Vec2<T>& operator/(const T& rhs) {
		Vec2<T> out(*this);
		out /= rhs;
		return *this;
	}

	Vec2<T>& operator+=(const Vec2<T>& rhs) {
		(*this)[0] += rhs[0];
		(*this)[1] += rhs[1];
		return *this;
	}

	Vec2<T>& operator-=(const Vec2<T>& rhs) {
		(*this)[0] -= rhs[0];
		(*this)[1] -= rhs[1];
		return *this;
	}

	bool operator==(const Vec2<T>& rhs) const {
		return (*this)[0] == rhs[0] && (*this)[1] == rhs[1];
	}

    T dot(Vec2 v) {
        T x = this->m_values[0] * v->m_values[0];
        T y = this->m_values[1] * v->m_values[1];

        return x + y;
    }
    T norm() {
        T x = static_cast<T>(pow(this->m_values[0], 2));
        T y = static_cast<T>(pow(this->m_values[1], 2));

        return static_cast<T>(sqrt(static_cast<double>(x) + static_cast<double>(y)));
    }
    Vec2 normalized() {
        return *this / norm();
    }


private:
	T m_values[2];
};

// These must be defined in the header file: https://isocpp.org/wiki/faq/templates#templates-defn-vs-decl
template<typename T>
inline Vec2<T> operator*(const T& lhs, Vec2<T> rhs) {
	rhs *= lhs;
	return rhs;
}

template<typename T>
inline Vec2<T> operator+(Vec2<T> lhs, const Vec2<T>& rhs) {
	lhs += rhs;
	return lhs;
}

template<typename T>
inline Vec2<T> operator-(Vec2<T> lhs, const Vec2<T>& rhs) {
	lhs -= rhs;
	return lhs;
}

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

	// Constructors
	Vec3() {}
	Vec3(T v1, T v2, T v3) {
		m_values[0] = v1;
		m_values[1] = v2;
		m_values[2] = v3;
	}

	// Operators
	T operator [] (unsigned i) const { return this->m_values[i]; }
	T& operator [] (unsigned i) { return this->m_values[i]; }
	T const * operator *() const { return this->m_values; }	// Used to pass raw data

	Vec3<T>& operator*=(const T& rhs) {
		(*this)[0] *= rhs;
		(*this)[1] *= rhs;
		(*this)[2] *= rhs;
		return *this;
	}

	Vec3<T>& operator*(const T& rhs) {
		Vec3<T> out(*this);
		out *= rhs;
		return out;
	}

	Vec3<T>& operator/=(const T& rhs) {
		(*this)[0] /= rhs;
		(*this)[1] /= rhs;
		(*this)[2] /= rhs;
		return *this;
	}

	Vec3<T>& operator/(const T& rhs) {
		Vec3<T> out(*this);
		out /= rhs;
		return out;
	}

	Vec3<T>& operator+=(const Vec3<T>& rhs) {
		(*this)[0] += rhs[0];
		(*this)[1] += rhs[1];
		(*this)[2] += rhs[2];
		return *this;
	}

	Vec3<T>& operator-=(const Vec3<T>& rhs) {
		(*this)[0] -= rhs[0];
		(*this)[1] -= rhs[1];
		(*this)[2] -= rhs[2];
		return *this;
	}

	bool operator==(const Vec3<T>& rhs) const {
		return (*this)[0] == rhs[0] && (*this)[1] == rhs[1] && (this)[2] == rhs[2];
	}

    T dot(Vec3 v) {
        T x = this->m_values[0] * v->m_values[0];
        T y = this->m_values[1] * v->m_values[1];
        T z = this->m_values[2] * v->m_values[2];

        return x + y + z;
    }

    T norm() {
        T x = static_cast<T>(pow(this->m_values[0], 2));
        T y = static_cast<T>(pow(this->m_values[1], 2));
        T z = static_cast<T>(pow(this->m_values[2], 2));

        return static_cast<T>(sqrt(static_cast<double>(x) + static_cast<double>(y) + static_cast<double>(z)));
    }

    Vec3 normalized() {
        return *this / norm();
    }


private:
	T m_values[3];

};

// These must be defined in the header file: https://isocpp.org/wiki/faq/templates#templates-defn-vs-decl
template<typename T>
inline Vec3<T> operator*(const T& lhs, Vec3<T> rhs) {
	rhs *= lhs;
	return rhs;
}

template<typename T>
inline Vec3<T> operator+(Vec3<T> lhs, const Vec3<T>& rhs) {
	lhs += rhs;
	return lhs;
}

template<typename T>
inline Vec3<T> operator-(Vec3<T> lhs, const Vec3<T>& rhs) {
	lhs -= rhs;
	return lhs;
}

// Common aliases for vector types
using Colour = Vec3<float>;
using ScreenCoord = Vec2<int>;
