#include "Settings.h"

namespace JSLib {

	Settings::DateProxy& Settings::DateProxy::operator=(const Date& d) {
		ObservableValue<Date>::operator=(d);
		return *this;
	}

	Settings::DateProxy::operator Date() const {
		return value();
	}

}