#pragma once
#include "MainHeader.hpp"

//#define TEST_DATA

class Main {
private:
	static const std::pair<const char*, const char*> FILES_ARG_STR;

public:
	explicit Main(int argc, char* argv[]) {
		this->InitArgv(argc, argv);
	}

	int LaunchMain() {
		Main::SetLocale(1251u);

		try {
			this->ParseArgs();
		}
		catch (const std::exception&) {
			fmt::print(fmt::fg(fmt::color::orange),
				"ВНИМАНИЕ! Добавьте файлы с входными данными...\nПример: {} {} fileName.json\n",
			           this->argv_.front(), Main::FILES_ARG_STR.second);

			return EXIT_FAILURE;
		}

		for (const auto& path : this->filePaths_) {
			try {
				fmt::print("Файл данных: ");
				fmt::print(fmt::fg(fmt::color::aqua), "{}\n", path);

				std::fstream file(path, std::fstream::in);

				if (!file.is_open()) {
					throw std::runtime_error("Wrong path or file doesn't exist!");
				}

				#if defined(TEST_DATA)
					const std::unique_ptr<IDataGetter> dataGetter
						= std::make_unique<TestExampleDataGetter>();
				#else
					const std::unique_ptr<IDataGetter> dataGetter
						= std::make_unique<JsonDataGetter>(std::move(file));
				#endif

				const auto points = dataGetter->GetData();
				const auto probability = dataGetter->GetConfidenceProbability();

				const std::unique_ptr<MathSolver<double>> mathSolver{
					new MathSolver<double>{points.begin(), points.end()}
				};

				const auto dataX = mathSolver->GetDataX();
				const auto dataY = mathSolver->GetDataY();
				const auto dataXX = mathSolver->GetDataXXProduct();
				const auto dataXY = mathSolver->GetDataXYProduct();
				const auto dataYY = mathSolver->GetDataYYProduct();

				const auto sumX = mathSolver->GetSumX();
				const auto sumY = mathSolver->GetSumY();
				const auto sumXX = mathSolver->GetSumXXProduct();
				const auto sumXY = mathSolver->GetSumXYProduct();
				const auto sumYY = mathSolver->GetSumYYProduct();

				const auto correlation = mathSolver->GetCorrelationCoefficient();
				const auto frequency = mathSolver->GetFrequency();
				const auto distribution = StudentsTable::GetInstance()
					.GetElement(1.0 - probability, dataX.size() - 2);

				const auto table = this->CreateTable
					(dataX, dataY, dataXX, dataXY, dataYY, sumX, sumY, sumXX, sumXY, sumYY);
				std::cout << table << '\n';

				Main::PrintDelimiter(std::cout, '=', 60u);

				std::cout << std::format("Коэффициент корреляции: {:.4f}", correlation) << '\n';
				std::cout << std::format("Уровень значимости: {:.4f}", frequency) << '\n';

				Main::PrintDelimiter(std::cout, '=', 60u);

				if (std::abs(frequency) <= distribution) {
					std::cout << std::format("|{:.4f}| <= {:.4f}", frequency, distribution) << '\n';
					std::cout << "Связь отсутствует." << '\n';
				}
				else {
					std::cout << std::format("|{:.4f}| > {:.4f}", frequency, distribution) << '\n';

					if (correlation >= -1.0 && correlation <= -0.5) {
						std::cout << "Вероятно, имеет место обратная связь." << '\n';
					}
					else if (correlation >= 0.5 && correlation <= 1.0) {
						std::cout << "Вероятно, имеет место прямая связь." << '\n';
					}
				}

				Main::PrintDelimiter(std::cout, '=', 60u);
				std::cout << std::flush;
			}
			catch (const std::exception& exception) {
				fmt::print(fmt::fg(fmt::color::red), "ОШИБКА!!! {}\n", exception.what());
			}
		}

		return EXIT_SUCCESS;
	}

private:
	std::vector<std::string> argv_;
	std::vector<std::string> filePaths_;

	void InitArgv(int argc, char* argv[]) {
		std::copy_n(argv, argc, std::back_inserter(this->argv_));
	}

	void ParseArgs() {
		try {
			const auto argumentParser = std::make_unique<argparse::ArgumentParser>();
			argumentParser->add_argument(Main::FILES_ARG_STR.first, Main::FILES_ARG_STR.second)
				.required()
				.nargs(static_cast<int>(this->argv_.size()) - 2)
				.help("specify the input JSON-files.");

			argumentParser->parse_args(this->argv_);

			this->filePaths_ = argumentParser->get<std::vector<std::string>>(Main::FILES_ARG_STR.first);
		}
		catch (const std::exception&) {
			throw;
		}
	}

	[[nodiscard]] tabulate::Table CreateTable(const std::vector<double>& dataX, const std::vector<double>& dataY,
	                                          const std::vector<double>& dataXX, const std::vector<double>& dataXY,
	                                          const std::vector<double>& dataYY,
	                                          double sumX, double sumY, double sumXX, double sumXY, double sumYY) const {
		tabulate::Table table;

		table.add_row({ "N", "X_i", "Y_i", "X_i * Y_i", "X_i^2", "Y_i^2" });
		for (auto i = 0u; i < dataX.size(); ++i) {
			table.add_row({
				std::format("{}", i + 1),
				std::format("{:.4f}", dataX.at(i)),
				std::format("{:.4f}", dataY.at(i)),
				std::format("{:.4f}", dataXY.at(i)),
				std::format("{:.4f}", dataXX.at(i)),
				std::format("{:.4f}", dataYY.at(i))
				});
		}
		table.add_row({
				std::format("sum:"),
				std::format("{:.4f}", sumX),
				std::format("{:.4f}", sumY),
				std::format("{:.4f}", sumXY),
				std::format("{:.4f}", sumXX),
				std::format("{:.4f}", sumYY)
			}
		);

		return table;
	}

	static void SetLocale(unsigned code) {
		#if defined(WIN32)
			SetConsoleCP(code);
			SetConsoleOutputCP(code);
		#endif
	}

	static void PrintDelimiter(std::ostream& out, const char symbol = '-', const size_t count = 40u) {
		out << std::string(count, symbol) << '\n';
	}
};

const std::pair<const char*, const char*> Main::FILES_ARG_STR = { "-f", "--files" };