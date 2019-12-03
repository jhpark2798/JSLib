#pragma once
#include "Date.h"
#include "..//Utility.h"
#include <memory>

namespace JSLib {

	class DayCounter {
	protected:
		class Impl {
		public:
			virtual ~Impl() {}
			virtual double yearFraction(const Date& d1, const Date& d2) const = 0;
		};
		explicit DayCounter(const std::shared_ptr<Impl>& impl) : impl_(impl) {}
		std::shared_ptr<Impl> impl_;
	public:
		DayCounter() {}
		double yearFraction(const Date&, const Date&) const;
	};

	

}