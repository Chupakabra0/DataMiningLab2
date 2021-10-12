#include "Main/Main.hpp"

int main(int argc, char* argv[]) {
	auto exitCode = EXIT_SUCCESS;

	try {
		exitCode = std::make_unique<Main>(argc, argv)->LaunchMain();
	}
	catch (const std::exception& exception) {
		fmt::print(fg(fmt::color::red), "Œÿ»¡ ¿!!! {}\n", exception.what());
	}

	return exitCode;
}
