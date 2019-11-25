#pragma once
#include "..//DesignPattern/Observer.h"
#include "..//Time/Date.h"

namespace JSLib {

	class TermStructure : public virtual Observer, public virtual Observable {
	public:

	protected:

	private:
		mutable Date referenceDate_;

	};

}