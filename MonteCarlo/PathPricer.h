#pragma once

namespace JSLib {

	template <class PathType, class ValueType=double>
	class PathPricer {
	public:
		virtual ~PathPricer() {}
		virtual ValueType operator()(const PathType& path) const = 0;
	};

}