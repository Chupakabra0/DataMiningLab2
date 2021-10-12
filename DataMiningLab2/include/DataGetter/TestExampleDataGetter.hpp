#pragma once
#include <memory>

#include "IDataGetter.hpp"

class TestExampleDataGetter : public IDataGetter {
private:
	static constexpr double CONFIDENCE_PROBABILITY = 0.95;
	static const std::unique_ptr<std::vector<Point<double>>> DATA;

public:
	[[nodiscard]] std::vector<Point<double>> GetData() const override {
		return *TestExampleDataGetter::DATA;
	}

	[[nodiscard]] double GetConfidenceProbability() const override {
		return TestExampleDataGetter::CONFIDENCE_PROBABILITY;
	}
};

const std::unique_ptr<std::vector<Point<double>>> TestExampleDataGetter::DATA
	= std::make_unique<std::vector<Point<double>>>(
		std::vector{
			Point<double>(2.7, 15.6),
			Point<double>(3.0, 15.3),
			Point<double>(2.8, 15.6),
			Point<double>(2.9, 15.2),
			Point<double>(2.6, 15.9),
			Point<double>(2.5, 16.1),
			Point<double>(2.8, 15.5),
			Point<double>(2.6, 16.0),
			Point<double>(2.5, 16.2)
		}
	);
	
