#pragma once

#include "MonteCarlo/McSimulation.h"
#include "VanillaOption.h"
#include "Math/GeneralStatistics.h"
#include "Time/Date.h"
#include "Null.h"
#include "DesignPattern/Observer.h"

namespace JSLib {

	template<template <class> class MCTraits, class RNGTraits,
		class S=GeneralStatistics, class Inst=VanillaOption>
		class McVanillaEngine : public Inst::engine, 
		public McSimulation<MCTraits, RNGTraits, S> {
		public:
			void calculate() const;
		protected:
			typedef typename McSimulation<MCTraits, RNGTraits, S>::path_generator_type
				path_generator_type;
			typedef typename McSimulation<MCTraits, RNGTraits, S>::path_pricer_type
				path_pricer_type;
			typedef typename McSimulation<MCTraits, RNGTraits, S>::stats_type
				stats_type;
			typedef typename McSimulation<MCTraits, RNGTraits, S>::result_type
				result_type;
			McVanillaEngine(const std::shared_ptr<StochasticProcess>&,
				size_t timeSteps, size_t timeStepsPerYear,
				bool brownianBridge, bool antitheticVariate, bool controlVariate,
				size_t requiredSamples, double requiredTolerance,
				size_t maxSamples, int seed);
			TimeGrid timeGrid() const override;
			std::shared_ptr<path_generator_type> pathGenerator() const override;
			result_type controlVariateValue() const override;

			std::shared_ptr<StochasticProcess> process_;
			size_t timeSteps_, timeStepsPerYear_;
			size_t requiredSamples_, maxSamples_;
			double requiredTolerance_;
			bool brownianBridge_;
			int seed_;
	};

	template<template <class> class MCTraits, class RNGTraits, class S, class Inst>
	void McVanillaEngine<MCTraits, RNGTraits, S, Inst>::calculate() const {
		McSimulation<MCTraits, RNGTraits, S>::calculate(requiredTolerance_,
			requiredSamples_, maxSamples_);
		this->results_.value = this->mcModel_->sampleAccumulator().mean();
		if (RNGTraits::allowsErrorEstimate)
			this->results_.errorEstimate = 
				this->mcModel_->sampleAccumulator().errorEstimate();
	}

	template<template <class> class MCTraits, class RNGTraits, class S, class Inst>
	McVanillaEngine<MCTraits, RNGTraits, S, Inst>::McVanillaEngine(
		const std::shared_ptr<StochasticProcess>& process,
		size_t timeSteps, size_t timeStepsPerYear,
		bool brownianBridge, bool antitheticVariate, bool controlVariate,
		size_t requiredSamples, double requiredTolerance,
		size_t maxSamples, int seed) 
		: McSimulation<MCTraits, RNGTraits, S>(antitheticVariate, controlVariate), 
			process_(process),	timeSteps_(timeSteps), timeStepsPerYear_(timeStepsPerYear), 
			requiredSamples_(requiredSamples), maxSamples_(maxSamples), 
			requiredTolerance_(requiredTolerance), brownianBridge_(brownianBridge), 
			seed_(seed) {
		JS_REQUIRE(timeSteps != static_cast<size_t>(Null<size_t>()) || 
			timeStepsPerYear != static_cast<size_t>(Null<size_t>()),
			"no time steps provided");
		JS_REQUIRE(timeSteps == static_cast<size_t>(Null<size_t>()) || 
			timeStepsPerYear == static_cast<size_t>(Null<size_t>()),
			"both time steps and time steps per year were provided");
		JS_REQUIRE(timeSteps != 0, 
			"timeSteps must be positive");
		JS_REQUIRE(timeStepsPerYear != 0,
			"timeStepsPerYear must be positive");
		// process_는 Observable이 아니라 등록이 안되고 Observable로 캐스팅하면
		// rvalue라 등록이 안되는것 같은데 어떻게 함?
		// this->registerWith(process_);
		// this->registerWith(std::dynamic_pointer_cast<Observable>(process_));
	}

	// 멤버변수로 TimeGrid를 가지고 있으면 되지 왜 새로 만드는지 모르겠음
	template<template <class> class MCTraits, class RNGTraits, class S, class Inst>
	TimeGrid McVanillaEngine<MCTraits, RNGTraits, S, Inst>::timeGrid() const {
		Date lastExerciseDate = this->arguments_.exercise->lastDate();
		double t = process_->time(lastExerciseDate);
		if (this->timeSteps_ != Null<size_t>()) {
			return TimeGrid(t, this->timeSteps_);
		} else if (this->timeStepsPerYear_ != Null<size_t>()) {
			size_t steps = static_cast<size_t>(this->timeStepsPerYear_ * t);
			return TimeGrid(t, std::max<size_t>(steps, 1));
		} else {
			JS_REQUIRE(false, "time steps not specified");
		}
	}

	template<template <class> class MCTraits, class RNGTraits, class S, class Inst>
	std::shared_ptr<typename 
		McVanillaEngine<MCTraits, RNGTraits, S, Inst>::path_generator_type> 
		McVanillaEngine<MCTraits, RNGTraits, S, Inst>::pathGenerator() const {
		size_t dimensions = process_->factors();
		TimeGrid grid = this->timeGrid();
		// 초기값을 제외한 다른 time node들을 만들기 위한 난수 생성 클래스 생성
		typename RNGTraits::rsg_type generator = 
			RNGTraits::make_sequence_generator(dimensions * (grid.size() - 1), seed_);
		return std::make_shared<path_generator_type>(process_, grid, generator, brownianBridge_);
	}

	template<template <class> class MCTraits, class RNGTraits, class S, class Inst>
	typename McVanillaEngine<MCTraits, RNGTraits, S, Inst>::result_type 
		McVanillaEngine<MCTraits, RNGTraits, S, Inst>::controlVariateValue() const {
		std::shared_ptr<PricingEngine> controlPE = this->controlPricingEngine();
		JS_REQUIRE(controlPE, "engine does not provide ");
		typename Inst::arguments* controlArguments = 
			dynamic_cast<typename Inst::arguments*>(controlPE->getArguments());
		JS_REQUIRE(controlArguments, "engine is using inconsistent arguments");
		*controlArguments = this->arguments_;
		controlPE->calculate();
		const typename Inst::results* controlResults =
			dynamic_cast<const typename Inst::results*>(controlPE->getResults());
		JS_REQUIRE(controlResults, "engine returns an inconsistent result type");
		return result_type(controlResults->value);
	}

}

