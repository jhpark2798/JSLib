#pragma once
#include "DesignPattern/Singleton.h"
#include "Time/Date.h"
#include "DesignPattern/ObservableValue.h"

namespace JSLib {

	class Settings : public Singleton<Settings> {
		friend class Singleton<Settings>;
	private:
		Settings() {}
		class DateProxy : public ObservableValue<Date> {
		public:
			DateProxy() {}
			DateProxy& operator=(const Date& d);
			operator Date() const;
		};
		DateProxy evaluationDate_;

	public:
		DateProxy& evaluationDate() { return evaluationDate_; }
		const DateProxy& evaluationDate() const { return evaluationDate_; }
	};

}