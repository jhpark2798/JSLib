#include "Actual365.h"

namespace JSLib {

	std::shared_ptr<DayCounter::Impl> Actual365::implementation() {
		std::shared_ptr<Impl> res = std::make_shared<Impl>();
		return std::static_pointer_cast<DayCounter::Impl>(res);
	}

}