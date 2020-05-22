#pragma once
#include "MonteCarloModel.h"
#include "../PricingEngine.h"

namespace JSLib {

	template <template <class> class MCtraits, class RNGtraits, class S = GeneralStatistics>
	class McSimulation {
	public:
		typedef typename MonteCarloModel<MCtraits, RNGtraits, S>::path_generator_type path_generator_type;
		typedef typename MonteCarloModel<MCtraits, RNGtraits, S>::path_pricer_type path_pricer_type;
		typedef typename MonteCarloModel<MCtraits, RNGtraits, S>::stats_type stats_type;
		typedef typename MonteCarloModel<MCtraits, RNGtraits, S>::result_type result_type;

		virtual ~McSimulation() {}

		result_type value(double tolerance, size_t maxSamples = (std::numeric_limits<int>::max)(), size_t minSamples = 1023) const;
		//tolerance까지 고려할 수 있는 valuation method
		result_type valueWithSamples(size_t samples) const;
		//단순히 sample수만 설정할 수 있는 valuation method
		result_type errorEstimate() const;
		const stats_type& sampleAccumulator() const;

		void calculate(double requiredTolerance, size_t requiredSamples, size_t maxSamples) const; //calculate는 value와 valueWithSamples로 구현되어 있음

	protected:
		//생성자가 protected인데 어떻게 사용할 수 있는지, 멤버 변수에 mcModel_이 있는데 이는 어떻게 초기화 하는지
		//->calculate method를 실행하면 mcModel_의 초기화가 이루어짐
		//이러면 사실 상 calculate를 실행하기 전에는 value, valueWithSamples, errorEstimate, sampleAccumulator는 사용할 수가 없는데 이런 게 public이어도 괜찮나?
		McSimulation(bool antitheticVariate, bool controlVariate) : antitheticVariate_(antitheticVariate), controlVariate_(controlVariate) {}
		virtual std::shared_ptr<path_pricer_type> pathPricer() const = 0;
		virtual std::shared_ptr<path_generator_type> pathGenerator() const = 0;
		virtual TimeGrid timeGrid() const = 0;
		virtual std::shared_ptr<path_pricer_type> controlPathPricer() const
		{
			return std::shared_ptr<path_pricer_type>();
		}
		virtual std::shared_ptr<path_generator_type> controlPathGenerator() const
		{
			return std::shared_ptr<path_generator_type>();
		}
		virtual std::shared_ptr<PricingEngine> controlPricingEngine() const
		{
			return std::shared_ptr<PricingEngine>();
		}
		virtual result_type controlVariateValue() const
		{
			return Null<result_type>();
		}
		template <class Sequence>
		static double maxError(const Sequence& sequence)
		{
			return *std::max_element(sequence.begin(), sequence.end());
		}
		static double maxError(double error)
		{
			return error;
		}

		mutable std::shared_ptr<MonteCarloModel<MCtraits, RNGtraits, S> > mcModel_;
		bool antitheticVariate_, controlVariate_;
	};

	template <template <class> class MCtraits, class RNGtraits, class S>
	inline typename McSimulation<MCtraits, RNGtraits, S>::result_type
		McSimulation<MCtraits, RNGtraits, S>::value(double tolerance, size_t maxSamples, size_t minSamples) const
	{
		size_t sampleNumber = mcModel_->sampleAccumulator().samples();
		if (sampleNumber < minSamples) {
			mcModel_->addSamples(minSamples - sampleNumber);
			sampleNumber = mcModel_->sampleAccumulator().samples();
		}

		size_t nextBatch;
		double order;
		result_type error(mcModel_->sampleAccumulator().errorEstimate());
		//errorEstimate()의 반환 타입이 double이니까 error는 무조건 double인데 왜 maxError 함수가 필요한 것인지 알 수 없음
		while (maxError(error) > tolerance) {
			JS_REQUIRE(sampleNumber < maxSamples, "max number of samples (" << maxSamples << ") reached, while error (" << error <<
				") is still above tolerance (" << tolerance << ")");

			order = maxError(error * error) / tolerance / tolerance;//몬테카를로의 error가 1/sqrt(n)에 비례함에 착안해, tolerance에 도달하기 위한 sample 값을 구함
			nextBatch = size_t(std::max<double>(static_cast<double>(sampleNumber)* order * 0.8 - static_cast<double>(sampleNumber),
				static_cast<double>(minSamples)));//0.8을 왜 곱하는 지?

			nextBatch = std::min(nextBatch, maxSamples - sampleNumber);//maxSamples를 초과하지 않도록 조정
			sampleNumber += nextBatch;
			mcModel_->addSamples(nextBatch);
			error = result_type(mcModel_->SampleAccumulator().errorEstimate());
		}

		return result_type(mcModel_->sampleAccumulator().mean());
	}

	template <template <class> class MCtraits, class RNGtraits, class S>
	inline typename McSimulation<MCtraits, RNGtraits, S>::result_type
		McSimulation<MCtraits, RNGtraits, S>::valueWithSamples(size_t samples) const
	{
		size_t sampleNumber = mcModel_->sampleAccumulator().samples();

		JS_REQUIRE(samples >= sampleNumber, "number of already simulated samples (" << sampleNumber << ") greater than requested samples (" << samples << ")");

		mcModel_->addSamples(samples - sampleNumber);

		return result_type(mcModel_->sampleAccumulator().mean());
	}

	template <template <class> class MCtraits, class RNGtraits, class S>
	inline void McSimulation<MCtraits, RNGtraits, S>::calculate(double requiredTolerance, size_t requiredSamples, size_t maxSamples) const
	{
		JS_REQUIRE(requiredTolerance != Null<double>() || requiredSamples != Null<size_t>(), "neither tolerance nor number of samples set");

		if (this->controlVariate_) {

			result_type controlVariateValue = this->controlVariateValue();
			JS_REQUIRE(controlVariateValue != Null<result_type>(), "engine does not provide control-variation price");

			std::shared_ptr<path_pricer_type> controlPP = this->controlPathPricer();
			JS_REQUIRE(controlPP, "engine does not provide control-variation path pricer");

			std::shared_ptr<path_generator_type> controlPG = this->controlPathGenerator();

			this->mcModel_ = std::shared_ptr<MonteCarloModel<MCtraits, RNGtraits, S> >
				(new MonteCarloModel<MCtraits, RNGtraits, S>(pathGenerator(), this->pathPricer(), stats_type(), this->antitheticVariate_, controlPP,
					controlVariateValue, controlPG));
		}
		else {
			this->mcModel_ = std::shared_ptr<MonteCarloModel<MCtraits, RNGtraits, S> >
				(new MonteCarloModel<MCtraits, RNGtraits, S>(pathGenerator(), this->pathPricer(), S(), this->antitheticVariate_));
		}

		if (requiredTolerance != Null<double>()) {
			if (maxSamples != Null<size_t>())
				this->value(requiredTolerance, maxSamples);
			else
				this->value(requiredTolerance);
		}
		else
			this->valueWithSamples(requiredSamples);
	}

	template <template <class> class MCtraits, class RNGtraits, class S>
	inline typename McSimulation<MCtraits, RNGtraits, S>::result_type
		McSimulation<MCtraits, RNGtraits, S>::errorEstimate() const
	{
		return mcModel_->sampleAccumulator().errorEstimate();
	}

	template <template <class> class MCtraits, class RNGtraits, class S>
	inline const typename McSimulation<MCtraits, RNGtraits, S>::stats_type&
		McSimulation<MCtraits, RNGtraits, S>::sampleAccumulator() const
	{
		return mcModel_->sampleAccumulator();
	}

}