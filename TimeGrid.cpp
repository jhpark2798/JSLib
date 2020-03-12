#include "TimeGrid.h"

namespace JSLib {

	TimeGrid::TimeGrid(double end, std::size_t steps) {
		JS_REQUIRE(end > 0.0, "negative times not allowed");
		double dt = end / steps;
		times_.reserve(steps + 1);
		for (std::size_t i = 0; i <= steps; ++i)
			times_.push_back(dt * i);
		mandatoryTimes_ = std::vector<double>(1);
		mandatoryTimes_[0] = end;
		dt_ = std::vector<double>(steps, dt);
	}

	std::size_t TimeGrid::index(double t) const {
		std::size_t i = closestIndex(t);
		if (close_enough(t, times_[i])) {
			return i;
		}
		else {
			throw std::runtime_error("using inadequate time grid");
		}
	}

	std::size_t TimeGrid::closestIndex(double t) const {
		const_iterator begin = times_.begin();
		const_iterator end = times_.end();
		const_iterator result = std::lower_bound(begin, end, t);
		if (result == begin) return 0;
		else if (result == end) return size() - 1;
		else {
			double dt1 = *result - t; // t와 오른쪽점 간격
			double dt2 = t - *(result - 1);	// t와 왼쪽점 간격
			if (dt1 < dt2) return result - begin;
			else return (result - begin) - 1;
		}
	}



}