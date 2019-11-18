#include "LazyObject.h"

namespace JSLib {

	void LazyObject::calculate() const {
		if (!calculated_) {
			calculated_ = true;
			try {
				performCalculations();
			}
			catch (...) {
				calculated_ = false;
				throw;
			}
		}
	}

}