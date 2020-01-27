#include "Actual365.h"

#include <memory>

namespace JSLib {

	std::shared_ptr<DayCounter::Impl> Actual365::implementation() {
		std::shared_ptr<DayCounter::Impl> res 
			= std::make_shared<Actual365::Impl>();
		return res;
	}

}