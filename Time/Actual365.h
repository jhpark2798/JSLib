#pragma once
#include "DayCounter.h"
#include <memory>

namespace JSLib {

	class Actual365 : public DayCounter {
	private:
		class Impl : public DayCounter::Impl {
		public:
			double yearFraction(const Date& d1, const Date& d2) const {
				return daysBetween(d1, d2) / 365.0;
			}
		};
		std::shared_ptr<DayCounter::Impl> implementation();
	public:
		explicit Actual365() {}
	};

}

