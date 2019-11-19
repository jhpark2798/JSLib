#pragma once
#include "DesignPattern/Singleton.h"
#include "Date.h"

namespace JSLib {

	class Settings : public Singleton<Settings> {
		friend class Singleton<Settings>;
	public:
		Date& evaluationDate() { return evaluationDate_; }
		const Date& evaluationDate() const { return evaluationDate_; }
	private:
		Settings() {}
		Date evaluationDate_;
	};

}