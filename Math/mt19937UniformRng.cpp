#include "mt19937UniformRng.h"

namespace JSLib {

	mt19937UniformRng::mt19937UniformRng(unsigned int seed)
		: rd(), gen(seed),	engine(0.0,1.0) {
		if (seed == 0)
			gen.seed(rd());
	}

	mt19937UniformRng::sample_type mt19937UniformRng::next() const {
		return mt19937UniformRng::sample_type(engine(gen), 1.0);
	}

	double mt19937UniformRng::nextReal() const {
		return engine(gen);
	}

}