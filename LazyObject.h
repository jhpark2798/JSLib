#pragma once

#include "Observer.h"

namespace JSLib {

	class LazyObject : public virtual Observer, public virtual Observable {
	public:
		LazyObject() {}
		~LazyObject() {}
		void update() { calculated_ = false; }
		virtual void calculate() const;
	protected:
		mutable bool calculated_;
		virtual void performCalculations() const = 0;
	};

}