#ifndef math_h
#define math_h

#include <cmath>
#include <cstdlib>

// MARK: - Generic math
template<class T> constexpr T toDegrees(const T& angleInRadians) noexcept {
	return 180/M_PI * angleInRadians;
}

template<class T> constexpr T toRadians(const T& angleInDegrees) noexcept {
	return M_PI/180 * angleInDegrees;
}

template<class T> constexpr T clamp(T value, T min, T max) noexcept {
	if( value < min ) return min;
	if( value > max ) return max;
	return value;
}

template<class T, class Real=float> constexpr T lerp(Real t, T start, T end) noexcept {
	return (1 - t) * start + t * end;
}

// MARK: - Vector2
template<class T> struct Vector2 {
	T x, y;
	
	constexpr Vector2() = default;
	constexpr Vector2(T x_, T y_) noexcept : x(x_), y(y_) {}
	
	[[nodiscard]] constexpr const T& operator[](std::size_t i) const noexcept { return (&x)[i]; }
	[[nodiscard]] constexpr T& operator[](std::size_t i) noexcept { return (&x)[i]; }
	
	[[nodiscard]] constexpr bool operator==(const Vector2& o) const noexcept { return (x == o.x) && (y == o.y); }
	[[nodiscard]] constexpr bool operator!=(const Vector2& o) const noexcept { return !operator==(o); }
	
	[[nodiscard]] constexpr Vector2 operator+() const noexcept { return *this; }
	[[nodiscard]] constexpr Vector2 operator-() const noexcept { return {-x, -y}; }
	
	[[nodiscard]] constexpr Vector2 operator+(const Vector2& o) const noexcept { return {x + o.x, y + o.y }; }
	[[nodiscard]] constexpr Vector2 operator-(const Vector2& o) const noexcept { return {x - o.x, y - o.y }; }
	[[nodiscard]] constexpr Vector2 operator*(const T& k) const noexcept { return {x * k, y * k}; }
	[[nodiscard]] constexpr Vector2 operator/(const T& k) const noexcept { return {x / k, y / k}; }
	
	constexpr Vector2& operator+=(const Vector2& o) noexcept { *this = *this + o; return *this; }
	constexpr Vector2& operator-=(const Vector2& o) noexcept { *this = *this - o; return *this; }
	constexpr Vector2& operator*=(const T& k) noexcept { *this = *this * k; return *this; }
	constexpr Vector2& operator/=(const T& k) noexcept { *this = *this / k; return *this; }
	
	[[nodiscard]] constexpr T length() const noexcept {
		return std::sqrt(x*x + y*y);
	}
	
	[[nodiscard]] constexpr Vector2 normalized() const noexcept {
		return *this / length();
	}
	
	[[nodiscard]] constexpr bool isHorizontal() const noexcept {
		return std::abs(x) > std::abs(y);
	}
	
	[[nodiscard]] constexpr bool isVertical() const noexcept {
		return std::abs(x) < std::abs(y);
	}
	
	constexpr std::size_t size() const noexcept { return Size; }
	static constexpr std::size_t Size = 2;
};

template<class T>
[[nodiscard]] constexpr Vector2<T> operator*(const T& k, const Vector2<T>& a) noexcept {
	return a * k;
}

using Vector2f = Vector2<float>;
using Vector2d = Vector2<double>;
using Vector2u = Vector2<unsigned long>;

// MARK: - Vector3
template<class T> struct Vector3 {
	T x, y, z;
	
	constexpr Vector3() = default;
	constexpr Vector3(T x_, T y_, T z_) noexcept : x(x_), y(y_), z(z_) {}
	constexpr Vector3(const Vector2<T>& v, float z) noexcept: x(v.x), y(v.y), z(z) {}
	
	[[nodiscard]] constexpr const T& operator[](std::size_t i) const noexcept { return (&x)[i]; }
	[[nodiscard]] constexpr T& operator[](std::size_t i) noexcept { return (&x)[i]; }
	
	[[nodiscard]] constexpr bool operator==(const Vector3& o) const noexcept { return (x == o.x) && (y == o.y) && (z == o.z); }
	[[nodiscard]] constexpr bool operator!=(const Vector3& o) const noexcept { return !operator==(o); }
	
	[[nodiscard]] constexpr Vector3 operator+() const noexcept { return *this; }
	[[nodiscard]] constexpr Vector3 operator-() const noexcept { return {-x, -y, -z}; }
	
	[[nodiscard]] constexpr Vector3 operator+(const Vector3& o) const noexcept { return {x + o.x, y + o.y, z + o.z }; }
	[[nodiscard]] constexpr Vector3 operator-(const Vector3& o) const noexcept { return {x - o.x, y - o.y, z - o.z }; }
	[[nodiscard]] constexpr Vector3 operator*(const T& k) const noexcept { return {x * k, y * k, z * k}; }
	[[nodiscard]] constexpr Vector3 operator/(const T& k) const noexcept { return {x / k, y / k, z / k}; }
	[[nodiscard]] constexpr Vector3 operator*(const Vector3& v) const noexcept { return {x * v.x, y * v.y, z * v.z}; }
	
	constexpr Vector3& operator+=(const Vector3& o) noexcept { *this = *this + o; return *this; }
	constexpr Vector3& operator-=(const Vector3& o) noexcept { *this = *this - o; return *this; }
	constexpr Vector3& operator*=(const T& k) noexcept { *this = *this * k; return *this; }
	constexpr Vector3& operator/=(const T& k) noexcept { *this = *this / k; return *this; }
	constexpr Vector3& operator*=(const Vector3& v) noexcept { *this = *this * v; return *this; }
	
	[[nodiscard]] constexpr T squaredLength() const noexcept {
		return x*x + y*y + z*z;
	}
	
	[[nodiscard]] constexpr T length() const noexcept {
		return std::sqrt(x*x + y*y + z*z);
	}
	
	[[nodiscard]] constexpr Vector3 normalized() const noexcept {
		return *this / length();
	}
	
	constexpr std::size_t size() const noexcept { return Size; }
	static constexpr std::size_t Size = 2;
};

template<class T>
[[nodiscard]] constexpr Vector3<T> operator*(const T& k, const Vector3<T>& a) noexcept {
	return a * k;
}

using Vector3f = Vector3<float>;
using Vector3d = Vector3<double>;

// MARK: - More math
template<class T>
[[nodiscard]] constexpr T dot(const Vector2<T>& a, const Vector2<T>& b) noexcept {
	return a.x * b.x + a.y * b.y;
}

template<class T>
[[nodiscard]] constexpr T dot(const Vector3<T>& a, const Vector3<T>& b) noexcept {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

template<class T>
[[nodiscard]] constexpr Vector3<T> cross(const Vector3<T>& a, const Vector3<T>& b) noexcept {
	return {
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	};
}

template<class T>
[[nodiscard]] constexpr T distance(Vector2<T> from, Vector2<T> to){
	return (to - from).length();
}

template<class T>
[[nodiscard]] constexpr T distance(Vector3<T> from, Vector3<T> to){
	return (to - from).length();
}

template<class T>
[[nodiscard]] constexpr Vector2<T> clamp(Vector2<T> v, Vector2<T> min, Vector2<T> max){
	return {
		clamp(v.x, min.x, max.x),
		clamp(v.y, min.y, max.y),
	};
}

template<class T>
[[nodiscard]] constexpr Vector3<T> clamp(Vector3<T> v, Vector3<T> min, Vector3<T> max){
	return {
		clamp(v.x, min.x, max.x),
		clamp(v.y, min.y, max.y),
		clamp(v.z, min.z, max.z),
	};
}
template<class T>
Vector3<T> randomInUnitSphere(){
	Vector3<T> p;
	
	do {
		p = T(2.0) * Vector3<T>(drand48(), drand48(), drand48()) - Vector3<T>{1, 1, 1};
	} while( p.squaredLength() >= 1.f );
	
	return p;
}

template<class T>
Vector3f reflect(const Vector3<T>& v, const Vector3<T>& n){
	return v - T(2) * dot(v, n) * n;
}

// MARK: - Ray
template<class T> struct Ray {
	Vector3<T> origin;
	Vector3<T> direction;
	
	Vector3<T> pointAt(T t) const noexcept {
		return origin + t * direction;
	}
};

using Rayf = Ray<float>;
using Rayd = Ray<double>;

#endif /* math_h */
