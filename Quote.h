#pragma once

#include "DesignPattern/Observer.h"

namespace JSLib {
	class Quote : public virtual Observable {
	public:
		virtual ~Quote() {}
		virtual double value() const = 0;
		virtual bool isValid() const = 0;
	};
}