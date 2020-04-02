#include "GeneralStatistics.h"
#include "functional.h"

namespace JSLib {

	GeneralStatistics::GeneralStatistics() {
		reset();
	}

	std::size_t GeneralStatistics::sampleSize() const {
		return samples_.size();
	}

	const std::vector<std::pair<double, double>>& 
		GeneralStatistics::data() const {
		return samples_;
	}

	double GeneralStatistics::weightSum() const {
		double result = 0.0;
		std::vector<std::pair<double, double>>::const_iterator it;
		for (it = samples_.begin(); it != samples_.end(); ++it) {
			result += it->second;
		}
		return result;
	}

	double GeneralStatistics::mean() const {
		std::size_t N = sampleSize();
		JS_REQUIRE(N != 0, "empty sample set");
		return expectationValue(identity<double>(), everywhere()).first;
	}

	double GeneralStatistics::variance() const {
		std::size_t N = sampleSize();
		JS_REQUIRE(N != 0, "empty sample set");
		double s2 = expectationValue(compose(square<double>(),
			subtract<double>(mean())), everywhere()).first;
		return s2 * N / (N - 1.0);
	}

	double GeneralStatistics::standardDeviation() const {
		return std::sqrt(variance());
	}

	double GeneralStatistics::errorEstimate() const {
		return std::sqrt(variance() / sampleSize());
	}

	double GeneralStatistics::skewness() const {
		// 미완성
		throw std::runtime_error("not supported");
		return 0;
	}

	double GeneralStatistics::kurtosis() const {
		// 미완성
		throw std::runtime_error("not supported");
		return 0;
	}

	double GeneralStatistics::min() const {
		JS_REQUIRE(sampleSize() > 0, "empty sample set");
		return std::min_element(samples_.begin(), samples_.end())->first;
	}

	double GeneralStatistics::max() const {
		JS_REQUIRE(sampleSize() > 0, "empty sample set");
		return std::max_element(samples_.begin(), samples_.end())->first;
	}

	double GeneralStatistics::percentile(double y) const {
		// 미완성
		throw std::runtime_error("not supported");
		return 0;
	}

	double GeneralStatistics::topPercentile(double y) const {
		// 미완성
		throw std::runtime_error("not supported");
		return 0;
	}

	void GeneralStatistics::add(double value, double weight) {
		JS_REQUIRE(weight >= 0.0, "negative weight not allowed");
		samples_.push_back(std::make_pair(value, weight));
		sorted_ = false;
	}

	void GeneralStatistics::reset() {
		samples_ = std::vector<std::pair<double, double>>();
		sorted_ = true;
	}

	void GeneralStatistics::reserve(std::size_t n) const {
		samples_.reserve(n);
	}

	void GeneralStatistics::sort() const {
		if (!sorted_) {
			std::sort(samples_.begin(), samples_.end());
			sorted_ = true;
		}
	}

}