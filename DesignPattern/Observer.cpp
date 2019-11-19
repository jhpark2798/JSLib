#include "Observer.h"

namespace JSLib {

	// Observable
	void Observable::notifyObservers() {
		for (auto i = observers_.begin(); i != observers_.end(); ++i) {
			(*i)->update();
		}
	}

	void Observable::registerObserver(Observer* o) {
		observers_.push_back(o);
	}

	void Observable::unregisterObserver(Observer* o) {
		auto i = std::find(observers_.begin(), observers_.end(), o);
		if (i != observers_.end()) observers_.erase(i);
	}

	// Observer
	Observer::~Observer() {
		for (auto i = observables_.begin(); i != observables_.end(); ++i) {
			(*i)->unregisterObserver(this);
		}
	}

	void Observer::registerWith(const std::shared_ptr<Observable>& o) {
		o->registerObserver(this);
		observables_.emplace_back(o);
	}

	void Observer::unregisterWith(const std::shared_ptr<Observable>& o) {
		o->unregisterObserver(this);
		auto i = std::find(observables_.begin(), observables_.end(), o);
		if (i != observables_.end()) observables_.erase(i);
	}

}
