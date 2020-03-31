#pragma once

#include "RandomSequenceGenerator.h"
#include "..//Math/mt19937NormalRng.h"

namespace JSLib {

	template <class GNG> // GaussianNumberGenerator
	struct GenericPseudoRandom {
		typedef GNG rng_type;
		typedef RandomSequenceGenerator<rng_type> rsg_type;
		enum { allowsErrorEstimate = 1 };
		static rsg_type make_sequence_generator(std::size_t dimension, int seed) {
			rng_type gng(seed);
			return rsg_type(dimension, gng);
		}
	};

	typedef GenericPseudoRandom<mt19937NormalRng> PseudoRandom;

}