#pragma once

#include <vector>

namespace JSLib {

	template <class RNG>
	class RandomSequenceGenerator {
	public:
		typedef Sample<std::vector<double>> sample_type;
		RandomSequenceGenerator(std::size_t dimensionality, const RNG& rng)
			: dimensionality_(dimensionality) rng_(rng),
			sequence_(std::vector<double>(dimensionality), 1.0) {}

		const sample_type& nextSequence() const {
			sequence_.weight = 1.0;
			for (std::size_t i = 0; i < dimensionality_; ++i) {
				typename RNG::sample_type x(rng_.next());
				sequence_.value[i] = x.value;
				sequence_.weight *= x.weight;
			}
			return sequence_;
		}
	
	private:
		std::size_t dimensionality_;
		RNG rng_;
		mutable sample_type sequence_;
	};

}