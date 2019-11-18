#pragma once

#include <vector>
#include "Date.h"

namespace JSLib {

	class Exercise {
	public:
		enum Type { American, Bermudan, European };
		explicit Exercise(Type type) : type_(type) {}
		virtual ~Exercise() {}
		
		Type type() const { return type_; }
		const std::vector<Date>& dates() const { return dates_; }
		Date date(int index) const { return dates_[index]; }
		Date lastDate() const { return dates_.back(); }

	protected:
		std::vector<Date> dates_;
		Type type_;
	};

	class EuropeanExercise : public Exercise {
	public:
		EuropeanExercise(const Date& date);
	};

}