#pragma once

#include "RNGTraits.h"
#include "PathGenerator.h"
#include "MultiPathGenerator.h"

namespace JSLib{

	template <class RNGTraits = PseudoRandom>
	struct SingleVariate {
		typedef RNGTraits rng_traits;
		typedef Path path_type;
		typedef PathPricer<path_type> path_pricer_type;
		typedef typename RNGTraits::rsg_type rsg_type;
		typedef PathGenerator<rsg_type> path_generator_type;
		enum { allowsErrorEstimate = RNGTraits::allowsErrorEstimate };
	};

	template <class RNGTraits = PseudoRandom>
	struct MultiVariate {
		typedef RNGTraits rng_traits;
		typedef MultiPath path_type;
		typedef PathPricer<path_type> path_pricer_type;
		typedef typename RNGTraits::rsg_type rsg_type;
		typedef MultiPathGenerator<rsg_type> path_generator_type;
		enum { allowsErrorEstimate = RNGTraits::allowsErrorEstimate };
	};

}