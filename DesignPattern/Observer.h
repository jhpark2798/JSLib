#pragma once

#include <vector>
#include <memory>

namespace JSLib {
	class Observer;

	class Observable {
		friend class Observer;
	public:
		Observable() {}
		virtual ~Observable() {}
		void notifyObservers();

	private:
		std::vector<Observer*> observers_;

		void registerObserver(Observer* o);
		void unregisterObserver(Observer* o);
	};

	class Observer {
	public:
		Observer() {}
		virtual ~Observer();
		void registerWith(const std::shared_ptr<Observable>& o);
		void unregisterWith(const std::shared_ptr<Observable>& o);
		virtual void update() = 0;
	private:
		std::vector<std::shared_ptr<Observable>> observables_;
	};

}