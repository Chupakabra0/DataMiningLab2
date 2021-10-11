#pragma once
#include <utility>

template<class T>
class Point {
private:
	using value_type = T;

public:
	template<class... Args>
	explicit Point(Args&&... args)
		: values_(std::forward<value_type>(args)...) {}

	Point(const Point&) = default;

	Point(Point&&) noexcept = default;

	virtual ~Point() noexcept = default;

	Point& operator=(const Point&) = default;

	Point& operator=(Point&&) noexcept = default;

	[[nodiscard]] value_type& x() const {
		return this->values_.first;
	}

	[[nodiscard]] value_type& y() const {
		return this->values_.second;
	}

	[[nodiscard]] const std::pair<value_type, value_type>& GetPair() const {
		return this->values_;
	}

private:
	std::pair<value_type, value_type> values_;
};
