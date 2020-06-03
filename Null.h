#pragma once
#include <boost/limits.hpp>
#include <boost/type_traits.hpp>

namespace JSLib {

	template <class Type>
	class Null;

	namespace detail {

		template <bool>
		struct FloatingPointNull; //true일 때는 float, false일 때는 int null array

		template <>
		struct FloatingPointNull<true> {
			static float nullValue() { return (std::numeric_limits<float>::max)(); }
		};

		template <>
		struct FloatingPointNull<false> {
			static int nullValue() { return (std::numeric_limits<int>::max)(); }
		};

	}

	template <typename T>
	class Null {
	public:
		Null() {}
		operator T() const { return T(detail::FloatingPointNull<boost::is_floating_point<T>::value>::nullValue()); }
		// operator T() const { return std::numeric_limits<T>::quiet_NaN(); }
	};

}