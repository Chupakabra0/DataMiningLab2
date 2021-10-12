#pragma once
#include <fstream>
#include <vector>

#include "nlohmann/json.hpp"

#include "IDataGetter.hpp"
#include "Point/Point.hpp"

class JsonDataGetter : public IDataGetter {
private:
	static constexpr auto* DATA_STR        = "data";
	static constexpr auto* PROBABILITY_STR = "confidence_probability";

public:
	explicit JsonDataGetter(std::fstream file) : file_(file) {
		this->ReadFile();
	}
	;
	JsonDataGetter(const JsonDataGetter&) = delete;

	JsonDataGetter(JsonDataGetter&&) noexcept = default;

	~JsonDataGetter() override = default;

	JsonDataGetter& operator=(const JsonDataGetter&) = default;

	JsonDataGetter& operator=(JsonDataGetter&&) noexcept = default;

	[[nodiscard]] std::vector<Point<double>> GetData() const override {
		const auto pairs
			= this->json_[JsonDataGetter::DATA_STR].get<std::vector<std::pair<double, double>>>();

		std::vector<Point<double>> result(pairs.size());

		std::ranges::for_each(pairs,
			[iter = result.begin()](const auto& pair) mutable  {
				*iter++ = Point<double>{ pair };
			}
		);

		return result;
	}

	[[nodiscard]] double GetConfidenceProbability() const override {
		return this->json_[JsonDataGetter::PROBABILITY_STR].get<double>();
	}

private:
	std::reference_wrapper<std::fstream> file_;
	nlohmann::json json_{};

	void ReadFile() {
		if (this->file_.get().is_open()) {
			this->file_ >> this->json_;
		}
	}
};