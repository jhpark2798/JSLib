#pragma once

namespace JSLib {

	template <class T>
	struct Sample {
	public:
		typedef T value_type;
		Sample(const T& value, double weight)
			: value(value), weight(weight) {}
		T value;
		double weight;
	};

}