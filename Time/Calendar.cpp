#include "Calendar.h"
#include "..//Utility.h"

namespace JSLib {

	Date Calendar::adjust(const Date& d) const {
		JS_REQUIRE(d != Date(), "null Date");
		Date d1 = d;	// BusinessDayConvention에 따라 d 값이 필요할 수도 있어서 이런식으로 짬
		while (isHoliday(d1)) {
			d1++;
		}
		return d1;
	}

	Date Calendar::advance(const Date& d, int n) const {
		JS_REQUIRE(d != Date(), "null Date");	// 지금은 time unit이 day인데, 확장여지가 남아있음
		Date d1 = d; // Unit에 따라 d 값이 필요할 수도 있어서 이런식으로 짬.
		for (int i = n; i > 0; --i) {
			d1++;
			while (isHoliday(d1)) 
				d1++;
		}
		return d1;
	}

}

