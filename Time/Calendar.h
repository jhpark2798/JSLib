#pragma once

#include "Date.h"
#include <memory>

namespace JSLib {

	class Calendar {
	protected:
		class Impl {
		public:
			virtual ~Impl() {}
			virtual bool isBusinessDay(const Date& d) const = 0;
		};
		std::shared_ptr<Impl> impl_;
	
	public:
		bool isBusinessDay(const Date& d) const { return impl_->isBusinessDay(d); }
		bool isHoliday(const Date& d) const { return !isBusinessDay(d); }

	};

}