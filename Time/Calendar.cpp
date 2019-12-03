#include "Calendar.h"
#include "..//Utility.h"

namespace JSLib {

	Date Calendar::adjust(const Date& d) const {
		JS_REQUIRE(d != Date(), "null Date");
		Date d1 = d;	// BusinessDayConvention�� ���� d ���� �ʿ��� ���� �־ �̷������� «
		while (isHoliday(d1)) {
			d1++;
		}
		return d1;
	}

	Date Calendar::advance(const Date& d, int n) const {
		JS_REQUIRE(d != Date(), "null Date");	// ������ time unit�� day�ε�, Ȯ�忩���� ��������
		Date d1 = d; // Unit�� ���� d ���� �ʿ��� ���� �־ �̷������� «.
		for (int i = n; i > 0; --i) {
			d1++;
			while (isHoliday(d1)) 
				d1++;
		}
		return d1;
	}

}

