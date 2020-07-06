#include "VanillaOption.h"
#include "Utility.h"
#include "Settings.h"
#include <stdexcept>

namespace JSLib {

	VanillaOption::VanillaOption(const std::shared_ptr<Payoff>& payoff,
		const std::shared_ptr<Exercise>& exercise) 
		: OneAssetOption(payoff, exercise) {}

}