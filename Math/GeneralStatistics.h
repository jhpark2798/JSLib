#pragma once

#include <vector>
#include <algorithm>
#include <utility>
#include <cmath>

#include "..//Utility.h"

namespace JSLib {

	class GeneralStatistics {
	public:
		typedef double value_type;
		GeneralStatistics();
		std::size_t sampleSize() const;
		const std::vector<std::pair<double, double>>& data() const;
		double weightSum() const;
		double mean() const;
		double variance() const;
		double standardDeviation() const;
		double errorEstimate() const;
		double skewness() const;
		double kurtosis() const;
		double min() const;
		double max() const;
		double percentile(double y) const;
		double topPercentile(double y) const;
		void add(double value, double weight = 1.0);

		template <class Func, class Predicate>
		std::pair<double, std::size_t> expectationValue(const Func& f,
			const Predicate& inRange) const {
			double num = 0.0, den = 0.0;
			std::size_t N = 0;
			std::vector<std::pair<double, double>>::const_iterator i;
			for (i = samples_.begin(); i != samples_.end(); ++i) {
				double x = i->first, w = i->second;
				if (inRange(x)) {
					num += f(x) * w;
					den += w;
					N += 1;
				}
			}
			if (N == 0)
				return std::make_pair<double, std::size_t>(NULL_DOUBLE, 0);
			else
				return std::make_pair(num / den, N);	// 왜 den으로 나눠주는지 모르겠음 
		}

		template <class DataIterator>
		void addSequence(DataIterator begin, DataIterator end) {
			for (; begin != end; ++begin)
				add(*begin);
		}

		template <class DataIterator, class WeightIterator>
		void addSequence(DataIterator begin, DataIterator end,
			WeightIterator wbegin) {
			for (; begin != end; ++begin, ++wbegin)
				add(*begin, *wbegin);
		}

		void reset();
		void reserve(std::size_t n) const;
		void sort() const;

	private:
		mutable std::vector<std::pair<double, double>> samples_;
		mutable bool sorted_;
	};

}