#pragma once

namespace JSLib {

	template <class PathType, class ValueType=double>
	class PathPricer {
	public:
		typedef PathType argument_type;
		typedef ValueType result_type;

		virtual ~PathPricer() {}
		virtual ValueType operator()(const PathType& path) const = 0;
	};

}