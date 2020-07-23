#pragma once

#include "..//MonteCarlo/Sample.h"

#include <random>

namespace JSLib {

	class mt19937NormalRng {
	public:
		explicit mt19937NormalRng(unsigned int seed = 0);
		// explicit mt19937NormalRng(const std::vector<unsigned int>& seeds);
		typedef Sample<double> sample_type;
		sample_type next() const;
		double nextReal() const;

	private:
		mutable std::mt19937_64 gen;
		mutable std::normal_distribution<double> engine;
	};

}