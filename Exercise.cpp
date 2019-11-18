#include "Exercise.h"
#include <stdexcept>
#include <vector>

namespace JSLib {

	EuropeanExercise::EuropeanExercise(const Date& date) 
		: Exercise(European) {
		dates_ = std::vector<Date>(1, date);
	}

}