#pragma once

#include "Sample.h"

#include <random>

namespace JSLib {

	class mt19937UniformRng {
	public:
		explicit mt19937UniformRng(unsigned int seed = 0);
		// explicit mt19937UniformRng(const std::vector<unsigned int>& seeds);
		typedef Sample<double> sample_type;
		sample_type next() const;
		double nextReal() const;
		// unsigned int nextInt32() const;

	private:
		std::random_device rd;
		mutable std::mt19937_64 gen;
		mutable std::uniform_real_distribution<> engine;
	};

}