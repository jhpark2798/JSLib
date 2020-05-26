#pragma once

#include "..//Math/GeneralStatistics.h"
#include "MonteCarloModel.h"
#include "..//TimeGrid.h"
#include "..//PricingEngine.h"

#include <limits>

namespace JSLib {

	template <template <class> class MCTraits, class RNGTraits, class S=GeneralStatistics>
	class McSimulation {
	public:
		typedef typename MonteCarloModel<MCTraits, RNGTraits, S>::path_generator_type
			path_generator_type;
		typedef typename MonteCarloModel<MCTraits, RNGTraits, S>::path_pricer_type
			path_pricer_type;
		typedef typename MonteCarloModel<MCTraits, RNGTraits, S>::stats_type
			stats_type;
		typedef typename MonteCarloModel<MCTraits, RNGTraits, S>::result_type
			result_type;
		virtual ~McSimulation() {}

		// simulation 횟수를 tolerance를 고려해서 적당히 늘려서 mean 값 리턴
		result_type value(double tolerance, 
			std::size_t maxSamples = std::numeric_limits<std::size_t>::max(),
			std::size_t minSamples = 1023) const;
		// sample size를 직접 설정하는 value 함수
		result_type valueWithSamples(std::size_t samples) const;
		result_type errorEstimate() const;
		const stats_type& sampleAccumulator() const;
		void calculate(double requiredTolerance, size_t requiredSamples, size_t maxSamples) const;
	protected:
		McSimulation(bool antitheticVariate, bool controlVairate)
			: antitheticVariate_(antitheticVariate), controlVariate_(controlVariate) {}
		virtual std::shared_ptr<path_pricer_type> pathPricer() const = 0;
		virtual std::shared_ptr<path_generator_type> pathGenerator() const = 0;
		virtual TimeGrid timeGrid() const = 0;
		virtual std::shared_ptr<path_pricer_type> controlPathPricer() const {
			return std::shared_ptr<path_pricer_type>();
		}
		virtual std::shared_ptr<path_generator_type> controlPathGenerator() const {
			return std::shared_ptr<path_generator_type>();
		}
		virtual std::shared_ptr<PricingEngine> controlPricingEngine() const {
			return std::shared_ptr<PricingEngine>();
		}
		virtual result_type controlVariateValue() const {
			return Null<result_type>();
		}
		// Statistics를 sequenceStatistic를 쓰면 maxError가 필요해짐
		template <class Sequence>
		static double maxError(const Sequence& sequence) {
			return *std::max_element(sequence.begin(), sequence.end());
		}
		static double maxError(double error) {
			return error;
		}

		mutable std::shared_ptr<MonteCarloModel<MCTraits, RNGTraits, S>> mcModel_;
		bool antitheticVariate_, controlVariate_;
	};

	template <template <class> class MCTraits, class RNGTraits, class S>
	typename McSimulation<MCTraits, RNGTraits, S>::result_type 
		McSimulation<MCTraits, RNGTraits, S>::value(double tolerance, 
			std::size_t maxSamples, std::size_t minSamples) const {
		size_t sampleNumber = mcModel_->sampleAccumulator().sampleSize();
		if (sampleNumber < minSamples) {
			mcModel_->addSamples(minSamples - sampleNumber);
			sampleNumber = mcModel_->sampleAccumulator().sampleSize();
		}
		size_t nextBatch;
		double order;
		result_type error(mcModel_->sampleAccumulator().errorEstimate());

		while (maxError(error) > tolerance) {
			JS_REQUIRE(sampleNumber < maxSamples, "non-proper sample size");
			order = maxError(error * error) / tolerance / tolerance;
			nextBatch = size_t(std::max<double>(
				static_cast<double>(sampleNumber) * order * 0.8 - static_cast<double>(sampleNumber),
				statc_cast<double>(minSamples)));
			sampleNumber += nextBatch;
			mcModel_->addSamples(nextBatch);
			error = result_type(mcModel_->sampleAccumulator().errorEstimate());
		}
		return result_type(mcModel_->sampleAccumulator().mean());
	}

	template <template <class> class MCTraits, class RNGTraits, class S>
	typename McSimulation<MCTraits, RNGTraits, S>::result_type
		McSimulation<MCTraits, RNGTraits, S>::valueWithSamples(std::size_t samples) const {
		size_t sampleNumber = mcModel_->sampleAccumulator().sampleSize();
		JS_REQUIRE(samples >= sampleNumber, "non-proper sample size");
		mcModel_->addSamples(samples - sampleNumber);
		return result_type(mcModel_->sampleAccumulator().mean());
	}

	template <template <class> class MCTraits, class RNGTraits, class S>
	typename McSimulation<MCTraits, RNGTraits, S>::result_type
		McSimulation<MCTraits, RNGTraits, S>::errorEstimate() const {
		return mcModel_->sampleAccumulator().errorEstimate();
	}

	template<template <class> class MCTraits, class RNGTraits, class S>
	typename const McSimulation<MCTraits, RNGTraits, S>::stats_type&
		McSimulation<MCTraits, RNGTraits, S>::sampleAccumulator() const {
		return mcModel_->sampleAccumulator();
	}

	template<template<class> class MCTraits, class RNGTraits, class S>
	void McSimulation<MCTraits, RNGTraits, S>::calculate(double requiredTolerance,
		size_t requiredSamples, size_t maxSamples) const {
		JS_REQUIRE(requiredTolerance != Null<double>() || requiredSamples != Null<size_t>(),
			"tolerance and sample size both are null");
		if (requiredTolerance != Null<double>()) {
			if (maxSamples != Null<size_t>())
				this->value(requiredTolerance, maxSamples);
			else
				this->value(requiredTolerance);
		}
		else
			this->valueWithSamples(requiredSamples);
	}

}