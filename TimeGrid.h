#pragma once

#include "Utility.h"
#include <vector>
#include <algorithm>
#include <numeric>

namespace JSLib {

	class TimeGrid {
	public:
		TimeGrid() {}
		TimeGrid(double end, std::size_t steps);

		// mandatoryTimes의 iterator를 받아서
		// times_에 mandatoryTimes의 point만 넣는 생성자
		template <class Iterator>
		TimeGrid(Iterator begin, Iterator end)
			: mandatoryTimes_(begin, end) {
			JS_REQUIRE(begin != end, "empty time sequence");
			std::sort(mandatoryTimes_.begin(), mandatoryTimes_.end());
			JS_REQUIRE(mandatoryTimes_.front() >= 0.0, "negative times not allowed");
			std::vector<double>::iterator e =
				std::unique(mandatoryTimes_.begin(), mandatoryTimes_.end(),
					static_cast<bool (*)(double, double)>(close_enough));
			mandatoryTimes_.resize(e - mandatoryTimes_.begin());
			if (mandatoryTimes_[0] > 0.0)
				times_.push_back(0.0);
			times_.insert(times_.end(), mandatoryTimes_.begin(), mandatoryTimes_.end());
			dt_.reserve(times_.size() - 1);
			std::adjacent_difference(times_.begin() + 1, times_.end(), 
				std::back_inserter(dt_));
		}

		template <class Iterator>
		TimeGrid(Iterator begin, Iterator end, std::size_t steps)
			: mandatoryTimes_(begin, end) {
			JS_REQUIRE(begin != end, "empty time sequence");
			std::sort(mandatoryTimes_.begin(), mandatoryTimes_.end());
			JS_REQUIRE(mandatoryTimes_.front() >= 0.0, "negative times not allowed");
			std::vector<double>::iterator e =
				std::unique(mandatoryTimes_.begin(), mandatoryTimes_.end(),
					static_cast<bool (*)(double, double)>(close_enough));
			mandatoryTimes_.resize(e - mandatoryTimes_.begin());
			double last = mandatoryTimes_.back();
			double dtMax;	// 무슨 역할?
			if (steps != 0) {
				dtMax = last / steps;
			}
			else {
				std::vector<double> diff;
				std::adjacent_difference(mandatoryTimes_.begin(), mandatoryTimes.end(),
					std::back_inserter(diff));
				if (diff.front() == 0.0)
					diff.erase(diff.begin());
				dtMax = *(std::min_element(diff.begin(), diff.end())); // 왜 이렇게?
			}

			double periodBegin = 0.0;
			times_.push_back(periodBegin);
			for (std::vector<double>::const_iterator t = mandatoryTimes_.begin();
				t != mandatoryTimes_.end(); ++t) {
				double periodEnd = *t;
				if (periodEnd != 0.0) {
					// nearest integer
					std::size_t nSteps = std::size_t((periodEnd - periodBegin) / dtMax + 0.5);
					nSteps = (nSteps != 0 ? nSteps : 1); // at least one time step
					double dt = (periodEnd - periodBegin) / nSteps;
					times_.reserve(nSteps);
					for (std::size_t n = 1; n <= nSteps; ++n)
						times_.push_back(periodBegin + n * dt);
				}
				periodBegin = periodEnd;
			}
			dt_.reserve(times_.size() - 1);
			std::adjacent_difference(times_.begin() + 1, times_.end(), 
				std::back_inserter(dt_));
		}
		// constructor end

		std::size_t index(double t) const; // returns the index i such that grid[i]=t
		std::size_t closestIndex(double t) const;	// returns index i s.t. grid[i] is closest to t
		double closestTime(double t) const { return times_[closestIndex(t)]; }
		const std::vector<double>& mandatoryTimes() const { return mandatoryTimes_; }
		double dt(std::size_t i) const { return dt_[i]; }

		typedef std::vector<double>::const_iterator const_iterator;
		typedef std::vector<double>::const_reverse_iterator const_reverse_iterator;
		
		// interface가 밖에서 값을 수정할 수 없게 되어 있음
		double operator[](std::size_t i) const { return times_[i]; }
		double at(std::size_t i) const { return times_.at(i); }
		std::size_t size() const { return times_.size(); }
		bool empty() const { return times_.empty(); }
		const_iterator begin() const { return times_.begin(); }
		const_iterator end() const { return times_.end(); }
		const_reverse_iterator rbegin() const { return times_.rbegin(); }
		const_reverse_iterator rend() const { return times_.rend(); }
		double front() const { return times_.front(); }
		double back() const { return times_.back(); }

	private:
		std::vector<double> times_;
		std::vector<double> dt_;
		std::vector<double> mandatoryTimes_;
	};

}