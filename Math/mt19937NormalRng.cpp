#include "mt19937NormalRng.h"

namespace JSLib {

	mt19937NormalRng::mt19937NormalRng(unsigned int seed)
		: gen(seed), engine(0.0, 1.0) {
		if (seed == 0)
			gen.seed(std::random_device{}());
	}

	mt19937NormalRng::sample_type mt19937NormalRng::next() const {
		return sample_type(engine(gen), 1.0);
	}

	double mt19937NormalRng::nextReal() const {
		return engine(gen);
	}

}