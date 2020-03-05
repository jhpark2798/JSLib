#pragma once

#include <random>

namespace JSLib {

	class RandomSequenceGenerator {
	public:

	private:
		size_t dimensionality_;
		std::mt19937 generator;
	};

}