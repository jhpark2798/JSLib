#pragma once
#include "Instrument.h"
#include "Payoff.h"
#include "Exercise.h"
#include "Time/Date.h"
#include "OneAssetOption.h"

#include <memory>

namespace JSLib {

	class VanillaOption : public OneAssetOption {
	public:
		VanillaOption(const std::shared_ptr<Payoff>&,
			const std::shared_ptr<Exercise>&);
	};

}