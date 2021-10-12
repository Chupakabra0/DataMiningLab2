#pragma once
#include <algorithm>
#include <vector>

#include "Point/Point.hpp"
#include "StudentsTable/StudentsTable.hpp"

template<class T> requires
	std::is_arithmetic_v<T> &&
	(!std::is_same_v<T, bool>) &&
	(!std::is_same_v<T, char>) &&
	(!std::is_same_v<T, unsigned char>)

class MathSolver final {
private:
	using value_type = T;

	static constexpr auto VECTOR_COUNT = 6u;

public:
	//-------------------------- CTORS & DTOR -------------------------------//
	template<class Iter>
	explicit MathSolver(Iter begin, Iter end)
		: data_(std::distance(begin, end)), xyProductVector_(this->data_.capacity()),
		  xxProductVector_(this->data_.capacity()), yyProductVector_(this->data_.capacity()) {
		this->FillData(begin, end);

		this->CalculateProductXX();
		this->CalculateProductXY();
		this->CalculateProductYY();

		this->CalculateSumX();
		this->CalculateSumY();
		this->CalculateSumXX();
		this->CalculateSumXY();
		this->CalculateSumYY();
	}

	MathSolver(const MathSolver&) = default;

	MathSolver(MathSolver&&) noexcept = default;

	~MathSolver() = default;

	//----------------------------- OPERATORS ----------------------------------//

	MathSolver& operator=(const MathSolver&) = default;

	MathSolver& operator=(MathSolver&&) noexcept = default;


	//-------------------- VECTORS' GETTERS -----------------------------------//

	[[nodiscard]] std::vector<value_type> GetDataX() const {
		std::vector<value_type> result(this->data_.size());

		std::for_each(this->data_.cbegin(), this->data_.cend(),
			[i = 0, &result](const Point<value_type>& point) mutable  {
				result[i++] = point.x();
			}
		);

		return result;
	}

	[[nodiscard]] std::vector<value_type> GetDataY() const {
		std::vector<value_type> result(this->data_.size());

		std::for_each(this->data_.cbegin(), this->data_.cend(),
			[i = 0, &result](const Point<value_type>& point) mutable  {
			result[i++] = point.y();
		}
		);

		return result;
	}

	[[nodiscard]] std::vector<value_type> GetDataXXProduct() const {
		return this->xxProductVector_;
	}

	[[nodiscard]] std::vector<value_type> GetDataXYProduct() const {
		return this->xyProductVector_;
	}

	[[nodiscard]] std::vector<value_type> GetDataYYProduct() const {
		return this->yyProductVector_;
	}

	//-------------------- SUMS' GETTERS -------------------------------------//

	[[nodiscard]] value_type GetSumX() const {
		return this->xSum_;
	}

	[[nodiscard]] value_type GetSumY() const {
		return this->ySum_;
	}

	[[nodiscard]] value_type GetSumXXProduct() const {
		return this->xxProductSum_;
	}

	[[nodiscard]] value_type GetSumXYProduct() const {
		return this->xyProductSum_;
	}

	[[nodiscard]] value_type GetSumYYProduct() const {
		return this->yyProductSum_;
	}

	//---------------------- MATH -----------------------------------------//

	[[nodiscard]] value_type GetCorrelationCoefficient() const {
		auto result = this->data_.size() * this->xyProductSum_ - this->xSum_ * this->ySum_;

		result /= std::sqrt((this->data_.size() * this->xxProductSum_ - std::pow(this->xSum_, 2.0)) *
			(this->data_.size() * this->yyProductSum_ - std::pow(this->ySum_, 2.0)));

		return result;
	}

	[[nodiscard]] value_type GetFrequency() const {
		const auto correlation = this->GetCorrelationCoefficient();
		auto result = correlation * std::sqrt(this->data_.size() - 2)
			/ std::sqrt(1 - std::pow(correlation, 2.0));

		return result;
	}

private:
	std::vector<Point<value_type>> data_;

	std::vector<value_type> xyProductVector_;
	std::vector<value_type> xxProductVector_;
	std::vector<value_type> yyProductVector_;

	value_type xSum_;
	value_type ySum_;
	value_type xxProductSum_;
	value_type xyProductSum_;
	value_type yyProductSum_;

	template<class Iter>
	void FillData(Iter begin, Iter end) {
		this->ForEach(begin, end,
			[i = 0, this](const Point<value_type>& point) mutable  {
			this->data_[i++] = point;
		}
		);
	}

	void CalculateProductXY() {
		this->ForEach(this->data_.cbegin(), this->data_.cend(),
			[i = 0, this](const Point<value_type>& point) mutable  {
				this->xyProductVector_[i++] = point.x() * point.y();
			}
		);
	}

	void CalculateProductXX() {
		this->ForEach(this->data_.cbegin(), this->data_.cend(), 
			[i = 0, this](const Point<value_type>& point) mutable  {
				this->xxProductVector_[i++] = point.x() * point.x();
			}
		);
	}

	void CalculateProductYY() {
		this->ForEach(this->data_.cbegin(), this->data_.cend(), 
			[i = 0, this](const Point<value_type>& point) mutable  {
				this->yyProductVector_[i++] = point.y() * point.y();
			}
		);
	}

	void CalculateSumX() {
		this->xSum_ = {};
		this->ForEach(this->data_.cbegin(), this->data_.cend(), 
			[this](const Point<value_type>& point) {
				this->xSum_ += point.x();
			}
		);
	}

	void CalculateSumY() {
		this->ySum_ = {};
		this->ForEach(this->data_.cbegin(), this->data_.cend(), 
			[this](const Point<value_type>& point) {
				this->ySum_ += point.y();
			}
		);
	}

	void CalculateSumXY() {
		this->xyProductSum_ = {};
		this->ForEach(this->xyProductVector_.cbegin(), this->xyProductVector_.cend(),
			[this](double val) {
				this->xyProductSum_ += val;
			}
		);
	}

	void CalculateSumXX() {
		this->xxProductSum_ = {};
		this->ForEach(this->xxProductVector_.cbegin(), this->xxProductVector_.cend(),
			[this](double val) {
				this->xxProductSum_ += val;
			}
		);
	}

	void CalculateSumYY() {
		this->yyProductSum_ = {};
		this->ForEach(this->yyProductVector_.cbegin(), this->yyProductVector_.cend(),
			[this](double val) {
				this->yyProductSum_ += val;
			}
		);
	}

	template<class Iter, class Func>
	void ForEach(Iter begin, Iter end, Func func) {
		if (this->data_.empty()) {
			return;
		}

		std::for_each(begin, end, func);
	}
};
