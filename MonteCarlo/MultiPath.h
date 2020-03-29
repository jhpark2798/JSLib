#pragma once
#include "Path.h"

namespace JSLib {

	class MultiPath {
	public:
		MultiPath() {}
		MultiPath(size_t nAsset, const TimeGrid& timeGrid);
		MultiPath(const std::vector<Path>& multiPath);

		size_t assetNumber() const { return multiPath_.size(); }
		size_t pathSize() const { return multiPath_[0].length(); }
		
		const Path& operator[](size_t j) const { return multiPath_[j]; }
		const Path& at(size_t j) const { return multiPath_.at(j); }
		Path& operator[](size_t j) { return multiPath_[j]; }
		Path& at(size_t j) { return multiPath_[j]; }

	private:
		std::vector<Path> multiPath_;
	};

	inline MultiPath::MultiPath(size_t nAsset, const TimeGrid& timeGrid) 
		: multiPath_(nAsset, Path(timeGrid)) {
		JS_REQUIRE(nAsset > 0, "number of asset must be positive");
	}

	inline MultiPath::MultiPath(const std::vector<Path>& multiPath) 
		: multiPath_(multiPath) {}

}