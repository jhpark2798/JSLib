#pragma once

namespace JSLib {

	class Extrapolator {
	public:
		Extrapolator() : extrapolate_(false) {}
		virtual ~Extrapolator() {}
		void enableExtrapolation(bool b = true) { extrapolate_ = b; }
		void disableExtrapolation(bool b = true) { extrapolate_ = !b; }
		bool allowsExtrapolation() const { return extrapolate_; }
	private:
		bool extrapolate_;
	};

}