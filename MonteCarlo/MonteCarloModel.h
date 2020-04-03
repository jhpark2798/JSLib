#pragma once
#include "MCTraits.h"
#include "../Math/GeneralStatistics.h"

namespace JSLib {

	template <template <class> class MC, class RNG, class S= GeneralStatistics>
	class MonteCarloModel {
	public:
		typedef MC<RNG> mc_traits;
		typedef RNG rng_traits;
		typedef typename MC<RNG>::path_generator_type path_generator_type;
		typedef typename MC<RNG>::path_pricer_type path_pricer_type;
		typedef typename path_generator_type::sample_type sample_type;
		typedef typename path_pricer_type::result_type result_type;
		typedef S stats_type;

		MonteCarloModel(const std::shared_ptr<path_generator_type>& pathGenerator, const std::shared_ptr<path_pricer_type>& pathPricer,
			const stats_type& sampleAccumulator, bool antitheticVariate, const std::shared_ptr<path_pricer_type>& cvPathPricer
			= std::shared_ptr<path_pricer_type>(), result_type cvOptionValue = result_type(), const std::shared_ptr<path_generator_type>& cvPathGenerator
			= std::shared_ptr<path_generator_type>()) : pathGenerator_(pathGenerator), pathPricer_(pathPricer), sampleAccumulator_(sampleAccumulator),
			isAntitheticVariate_(antitheticVariate), cvPathPricer_(cvPathPricer), cvOptionValue_(cvOptionValue), cvPathGenerator_(cvPathGenerator)
		{
			if (!cvPathPricer_)
				isControlVariate_ = false;
			else
				isControlVariate_ = true;
		}
		void addSamples(size_t samples);
		const stats_type& sampleAccumulator() const;

	private:
		std::shared_ptr<path_generator_type> pathGenerator_;
		std::shared_ptr<path_pricer_type> pathPricer_;
		stats_type sampleAccumulator_;
		bool isAntitheticVariate_;
		std::shared_ptr<path_pricer_type> cvPathPricer_;
		result_type cvOptionValue_;
		bool isControlVariate_;
		std::shared_ptr<path_generator_type> cvPathGenerator_;

	};

	template <template <class> class MC, class RNG, class S>
	void MonteCarloModel<MC, RNG, S>::addSamples(size_t samples)
	{
		for (size_t j = 1; j <= samples; ++j) {
			const sample_type& path = pathGenerator_->next();
			result_type price = (*pathPricer_)(path.value);

			if (isControlVariate_) {
				if (!cvPathGenerator_) {
					price += cvOptionValue_ - (*cvPathPricer_)(path.value);
				}
				else {
					const sample_type& cvPath = cvPathGenerator_->next();
					price += cvOptionValue_ - (*cvPathPricer_)(cvPath.value);
				}
			}

			if (isAntitheticVariate_) {
				const sample_type& atPath = pathGenerator_->antithetic();
				result_type price2 = (*pathPricer_)(atPath.value);
				if (isControlVariate_) {
					if (!cvPathGenerator_) {
						price2 += cvOptionValue_ - (*cvPathPricer_)(atPath.value);
					}
					else {
						const sample_type& cvPath = cvPathGenerator_->antithetic();
						price2 += cvOptionValue_ - (*cvPathPricer_)(cvPath.value);
					}
				}

				sampleAccumulator_.add((price + price2) / 2.0, path.weight);
			}
			else
				sampleAccumulator_.add(price, path.weight);
		}
	}

	template <template <class> class MC, class RNG, class S>
	inline const typename MonteCarloModel<MC, RNG, S>::stats_type& MonteCarloModel<MC, RNG, S>::sampleAccumulator() const
	{
		return sampleAccumulator_;
	}

}