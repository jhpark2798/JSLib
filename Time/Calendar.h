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
		Date adjust(const Date& d) const;	// 가장 가까운 미래의 영업일 계산
		Date advance(const Date& d, int n) const;	// n영업일 후 계산
	};

}