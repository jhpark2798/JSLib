#pragma once

#include "McVanillaEngine.h"
#include "Math/GeneralStatistics.h"
#include "MonteCarlo/PathPricer.h"
#include "MonteCarlo/Path.h"
#include "Utility.h"
#include "Payoff.h"
#include "Null.h"
#include "VanillaOption.h"
#include "Option.h"
#include "Process/GeneralizedBlackScholesProcess.h"

#include <memory>

namespace JSLib {
	template<class RNGTraits, class S=GeneralStatistics>
	class McEuropeanEngine : public McVanillaEngine<SingleVariate, RNGTraits, S> {
	public:
		typedef typename McVanillaEngine<SingleVariate, RNGTraits, S>::path_generator_type
			path_generator_type;
		typedef typename McVanillaEngine<SingleVariate, RNGTraits, S>::path_pricer_type
			path_pricer_type;
		typedef typename McVanillaEngine<SingleVariate, RNGTraits, S>::stats_type
			stats_type;
		McEuropeanEngine(
			const std::shared_ptr<GeneralizedBlackScholesProcess>& process,
			size_t timeSteps, size_t timeStepsPerYear,
			bool brownianBridge, bool antitheticVariate,
			size_t requiredSamples, double requiredTolerance,
			size_t maxSamples, int seed);
	protected:
		std::shared_ptr<path_pricer_type> pathPricer() const override;
	};

	// 객체 생성을 편하게, 가독성 좋게 해주는 팩토리 클래스
	template <class RNGTraits=PseudoRandom, class S=GeneralStatistics>
	class MakeMcEuropeanEngine {
	public:
		MakeMcEuropeanEngine(const std::shared_ptr<GeneralizedBlackScholesProcess>&);
		MakeMcEuropeanEngine& withSteps(size_t steps);
		MakeMcEuropeanEngine& withStepsPerYear(size_t steps);
		MakeMcEuropeanEngine& withBrownianBridge(bool b = true);
		MakeMcEuropeanEngine& withSamples(size_t samples);
		MakeMcEuropeanEngine& withAbsoluteTolerance(double tolerance);
		MakeMcEuropeanEngine& withMaxSamples(size_t samples);
		MakeMcEuropeanEngine& withSeed(int seed);
		MakeMcEuropeanEngine& withAntitheticVariate(bool b = true);

		operator std::shared_ptr<PricingEngine>() const;
	private:
		std::shared_ptr<GeneralizedBlackScholesProcess> process_;
		bool antithetic_;
		size_t steps_, stepsPerYear_, samples_, maxSamples_;
		double tolerance_;
		bool brownianBridge_;
		int seed_;
	};

	class EuropeanPathPricer : public PathPricer<Path> {
	public:
		EuropeanPathPricer(std::shared_ptr<StrikedTypePayoff>, double discount);
		double operator()(const Path& path) const override;
	private:
		std::shared_ptr<StrikedTypePayoff> payoff_;
		double discount_;
	};

	template <class RNGTraits, class S>
	McEuropeanEngine<RNGTraits, S>::McEuropeanEngine(
		const std::shared_ptr<GeneralizedBlackScholesProcess>& process,
		size_t timeSteps, size_t timeStepsPerYear,
		bool brownianBridge, bool antitheticVariate,
		size_t requiredSamples, double requiredTolerance,
		size_t maxSamples, int seed)
		: McVanillaEngine<SingleVariate, RNGTraits, S>(
			process, timeSteps, timeStepsPerYear, brownianBridge, antitheticVariate, false,
			requiredSamples, requiredTolerance, maxSamples, seed) {}

	template <class RNGTraits, class S>
	std::shared_ptr<typename McEuropeanEngine<RNGTraits, S>::path_pricer_type>
		McEuropeanEngine<RNGTraits, S>::pathPricer() const {
		std::shared_ptr<StrikedTypePayoff> payoff = // check payoff type
			std::dynamic_pointer_cast<StrikedTypePayoff>(this->arguments_.payoff);
		JS_REQUIRE(payoff, "non-striked type payoff given");
		// process는 체크 안해도 되는거 아닌가?
		// 해야함. this->process_는 StochasticProcess임. 하지만 여기서는
		// GeneralizedBlackScholesProcess의 인터페이스를 사용하고 싶기 때문.
		std::shared_ptr<GeneralizedBlackScholesProcess> process = // check process type
			std::dynamic_pointer_cast<GeneralizedBlackScholesProcess>(this->process_);
		JS_REQUIRE(process, "geometric brownian motion process required");
		auto pricer = std::make_shared<EuropeanPathPricer>(
			payoff, process->riskFreeRate()->discount(this->timeGrid().back())
			);
		return pricer;
	}

	template <class RNGTraits, class S>
	MakeMcEuropeanEngine<RNGTraits, S>::MakeMcEuropeanEngine(
		const std::shared_ptr<GeneralizedBlackScholesProcess>& process) 
		:  process_(process), antithetic_(false),
			steps_(Null<size_t>()), stepsPerYear_(Null<size_t>()), 
			samples_(Null<size_t>()), maxSamples_(Null<size_t>()), 
			tolerance_(Null<double>()), brownianBridge_(false), seed_(0) {}

	template <class RNGTraits, class S>
	MakeMcEuropeanEngine<RNGTraits, S>& 
		MakeMcEuropeanEngine<RNGTraits, S>::withSteps(size_t steps) {
		steps_ = steps;
		return *this;
	}

	template <class RNGTraits, class S>
	MakeMcEuropeanEngine<RNGTraits, S>& 
		MakeMcEuropeanEngine<RNGTraits, S>::withStepsPerYear(size_t steps) {
		stepsPerYear_ = steps;
		return *this;
	}

	template <class RNGTraits, class S>
	MakeMcEuropeanEngine<RNGTraits, S>&
		MakeMcEuropeanEngine<RNGTraits, S>::withBrownianBridge(bool b) {
		brownianBridge_ = b;
		return *this;
	}

	template <class RNGTraits, class S>
	MakeMcEuropeanEngine<RNGTraits, S>&
		MakeMcEuropeanEngine<RNGTraits, S>::withSamples(size_t samples) {
		// tolerance, samples size 중 하나만 설정해야 함.
		JS_REQUIRE(tolerance_ == Null<double>(), "tolerance already set");
		samples_ = samples;
		return *this;
	}

	template <class RNGTraits, class S>
	MakeMcEuropeanEngine<RNGTraits, S>&
		MakeMcEuropeanEngine<RNGTraits, S>::withAbsoluteTolerance(double tolerance) {
		JS_REQUIRE(samples_ == Null<size_t>(), "number of samples already set");
		JS_REQUIRE(RNGTraits::allowsErrorEstimate, 
			"chosen random generator policy does not allow an error estimate");
		tolerance_ = tolerance;
		return *this;
	}

	template <class RNGTraits, class S>
	MakeMcEuropeanEngine<RNGTraits, S>&
		MakeMcEuropeanEngine<RNGTraits, S>::withMaxSamples(size_t samples) {
		maxSamples_ = samples;
		return *this;
	}

	template <class RNGTraits, class S>
	MakeMcEuropeanEngine<RNGTraits, S>&
		MakeMcEuropeanEngine<RNGTraits, S>::withSeed(int seed) {
		seed_ = seed;
		return *this;
	}

	template <class RNGTraits, class S>
	MakeMcEuropeanEngine<RNGTraits, S>&
		MakeMcEuropeanEngine<RNGTraits, S>::withAntitheticVariate(bool b) {
		antithetic_ = b;
		return *this;
	}

	template <class RNGTraits, class S>
	MakeMcEuropeanEngine<RNGTraits, S>::operator std::shared_ptr<PricingEngine>() const {
		JS_REQUIRE(steps_ != Null<size_t>() || stepsPerYear_ != Null<size_t>(),
			"number of steps not given");
		JS_REQUIRE(steps_ == Null<size_t>() || stepsPerYear_ == Null<size_t>(),
			"number of steps overspecified");
		std::shared_ptr<PricingEngine> engine =
			std::make_shared<McEuropeanEngine<RNGTraits, S>>(
				process_, steps_, stepsPerYear_, brownianBridge_,
				antithetic_, samples_, tolerance_, maxSamples_, seed_);
		return engine;
	}

	EuropeanPathPricer::EuropeanPathPricer(std::shared_ptr<StrikedTypePayoff> payoff, double discount)
		: payoff_(payoff), discount_(discount) {
		JS_REQUIRE(payoff->strike() >= 0.0, "strike less than zero not allowed");
	}

	double EuropeanPathPricer::operator()(const Path& path) const {
		JS_REQUIRE(path.length() > 0, "the path cannot be empty");
		return payoff_->operator()(path.back()) * discount_;
	}

}


