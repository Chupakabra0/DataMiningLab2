#pragma once
#include <algorithm>
#include <array>
#include <numeric>
#include <map>

class StudentsTable {
private:
	static constexpr auto ALPHA_SIZE = 8u;
	static constexpr auto K_SIZE     = 35u;
	static constexpr auto TOTAL_SIZE = StudentsTable::ALPHA_SIZE * StudentsTable::K_SIZE;

public:
	~StudentsTable() = default;

	StudentsTable(const StudentsTable&) = delete;

	StudentsTable(StudentsTable&&) noexcept = default;

	StudentsTable& operator=(const StudentsTable&) = default;

	StudentsTable& operator=(StudentsTable&&) noexcept = default;

	static StudentsTable& GetInstance() {
		static StudentsTable instance;
		return instance;
	}

	[[nodiscard]] double GetElement(const double sigma, const unsigned k) const {
		auto result = 0.0;

		if (k < this->kTable_.front() || k > this->kTable_.back()) {
			return result;
		}
		if (sigma > this->alphaTable_.front() || sigma < this->alphaTable_.back()) {
			return result;
		}

		const auto lowerBound = this->map_.lower_bound(k)->second.lower_bound(sigma);
		const auto prev = std::prev(lowerBound);

		return std::abs(sigma - prev->first) < std::abs(lowerBound->first - sigma) ? prev->second : lowerBound->second;
	}

private:
	std::array<double, StudentsTable::TOTAL_SIZE> totalTable_ {
		3.078, 6.314, 12.71, 31.82, 63.66, 127.3, 318.3, 636.6,   // k = 1
	    1.886, 2.920, 4.303, 6.965, 9.925, 14.09, 22.33, 31.60,   // k = 2
	    1.638, 2.353, 3.182, 4.541, 5.841, 7.453, 10.21, 12.92,   // k = 3
	    1.533, 2.132, 2.776, 3.747, 4.604, 5.598, 7.173, 8.610,   // k = 4
	    1.476, 2.015, 2.571, 3.365, 4.032, 4.773, 5.894, 6.869,   // k = 5
	    1.440, 1.943, 2.447, 3.143, 3.707, 4.317, 5.208, 5.959,   // k = 6
	    1.415, 1.895, 2.365, 2.998, 3.499, 4.029, 4.785, 5.408,   // k = 7
	    1.397, 1.860, 2.306, 2.698, 3.355, 3.893, 4.501, 5.041,   // k = 8
	    1.383, 1.833, 2.262, 2.821, 3.250, 3.690, 4.297, 4.781,   // k = 9
	    1.372, 1.812, 2.228, 2.764, 3.169, 3.581, 4.144, 4.587,   // k = 10
	    1.363, 1.796, 2.201, 2.718, 3.106, 3.497, 4.025, 4.437,   // k = 11
	    1.356, 1.782, 2.179, 2.681, 3.055, 3.428, 3.930, 4.318,   // k = 12
	    1.350, 1.771, 2.160, 2.650, 3.012, 3.372, 3.852, 4.221,   // k = 13
	    1.345, 1.761, 2.145, 2.624, 2.977, 3.326, 3.787, 4.140,   // k = 14
	    1.341, 1.753, 2.131, 2.602, 2.947, 3.286, 3.733, 4.073,   // k = 15
	    1.337, 1.746, 2.120, 2.583, 2.921, 3.252, 3.686, 4.015,   // k = 16
	    1.333, 1.740, 2.110, 2.567, 2.898, 3.222, 3.646, 3.965,   // k = 17
	    1.330, 1.734, 2.101, 2.552, 2.878, 3.197, 3.610, 3.922,   // k = 18
	    1.328, 1.729, 2.093, 2.539, 2.861, 3.174, 3.579, 3.833,   // k = 19
	    1.325, 1.725, 2.086, 2.528, 2.845, 3.153, 3.552, 3.850,   // k = 20
	    1.323, 1.721, 2.080, 2.518, 2.831, 3.135, 3.527, 3.819,   // k = 21
	    1.321, 1.717, 2.074, 2.508, 2.819, 3.119, 3.505, 3.792,   // k = 22
	    1.319, 1.714, 2.069, 2.500, 2.807, 3.104, 3.485, 3.768,   // k = 23
	    1.318, 1.711, 2.064, 2.492, 2.797, 3.091, 3.467, 3.745,   // k = 24
	    1.316, 1.708, 2.060, 2.485, 2.787, 3.078, 3.450, 3.725,   // k = 25
	    1.315, 1.706, 2.056, 2.479, 2.779, 3.067, 3.435, 3.707,   // k = 26
	    1.314, 1.703, 2.052, 2.473, 2.771, 3.057, 3.421, 3.689,   // k = 27
	    1.313, 1.701, 2.048, 2.467, 2.763, 3.047, 3.408, 3.674,   // k = 28
	    1.311, 1.699, 2.045, 2.462, 2.756, 3.038, 3.396, 3.660,   // k = 29
	    1.310, 1.697, 2.042, 2.457, 2.750, 8.030, 3.385, 3.646,   // k = 30
	    1.303, 1.684, 2.021, 2.423, 2.704, 2.971, 3.307, 3.551,   // k = 40
	    1.296, 1.671, 2.000, 2.390, 2.660, 2.915, 3.232, 3.460,   // k = 60
	    1.290, 1.660, 1.984, 2.864, 2.626, 2.871, 3.174, 3.390,   // k = 100
	    1.289, 1.658, 1.980, 2.358, 2.617, 2.860, 3.160, 3.373,   // k = 120
		1.282, 1.645, 1.960, 2.327, 2.576, 2.808, 3.091, 3.291 	  // k = 10'000
	//  0.200  0.100  0.050  0.025  0.012  0.005  0.002  0.001
	};

	std::array<unsigned, StudentsTable::K_SIZE> kTable_{};

	std::array<double, StudentsTable::ALPHA_SIZE> alphaTable_{};

	std::map<unsigned, std::map<double, double>> map_{};

	StudentsTable() {
		this->FillKTable();
		this->FillAlphaTable();
		this->FillMap();
	}

	void FillKTable() {
		constexpr auto offset = 30u;
		auto endOffsetIter    = std::next(this->kTable_.begin(), offset);

		std::iota(this->kTable_.begin(), endOffsetIter, 1u);

		*endOffsetIter++ = 40u;
		*endOffsetIter++ = 60u;
		*endOffsetIter++ = 100u;
		*endOffsetIter++ = 120u;
		*endOffsetIter++ = 10'000u;
	}

	void FillAlphaTable() {
		std::ranges::generate(this->alphaTable_, [alpha = 0.2]() mutable {
			const auto oldAlpha = alpha;
			alpha /= 2;
			return oldAlpha;
		});
	}

	void FillMap() {
		auto totalIter = this->totalTable_.begin();

		std::ranges::for_each(std::as_const(this->kTable_), [&totalIter, this](auto k) {
			std::ranges::for_each(std::as_const(this->alphaTable_),[&totalIter, k, this](auto alpha) {
					this->map_[k][alpha] = *totalIter++;
				}
			);
		});
	}
};
