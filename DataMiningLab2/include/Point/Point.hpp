#pragma once
#include <utility>

template<class T>
class Point {
private:
	using value_type = T;

public:
	template<class... Args>
	explicit Point(Args... args)
		: values_(std::forward<Args>(args)...) {}

	Point(const Point&) = default;

	Point(Point&&) noexcept = default;

	virtual ~Point() noexcept = default;

	Point& operator=(const Point&) = default;

	Point& operator=(Point&&) noexcept = default;

	[[nodiscard]] value_type x() const {
		return this->values_.first;
	}

	[[nodiscard]] value_type y() const {
		return this->values_.second;
	}

	[[nodiscard]] std::pair<value_type, value_type> GetPair() const {
		return this->values_;
	}

	void SetX(value_type x) {
		this->values_.first = x;
	}

	void SetY(value_type y) {
		this->values_.second = y;
	}

private:
	std::pair<value_type, value_type> values_;
};
