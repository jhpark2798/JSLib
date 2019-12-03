#pragma once

#include "Observer.h"
#include <memory>

namespace JSLib {

	template <class T>
	class ObservableValue {
	public:
		ObservableValue();
		ObservableValue(const T&);
		ObservableValue(const ObservableValue<T>&);
		ObservableValue<T>& operator=(const T&);
		ObservableValue<T>& operator=(const ObservableValue<T>&);

		operator T() const;
		operator std::shared_ptr<Observable>() const;
		const T& value() const;
	private:
		T value_;
		std::shared_ptr<Observable> observable_;
	};

	template <class T>
	ObservableValue<T>::ObservableValue()
		: value_(), observable_(std::make_shared<Observable>()) {}

	template <class T>
	ObservableValue<T>::ObservableValue(const T& t)
		: value_(t), observable_(std::make_shared<Observable>()) {}

	template <class T>
	ObservableValue<T>::ObservableValue(const ObservableValue<T>& t)
		: value_(t.value_), observable_(std::make_shared<Observable>()) {}

	template <class T>
	ObservableValue<T>& ObservableValue<T>::operator=(const T& t) {
		value_ = t;
		observable_->notifyObservers();
		return *this;
	}

	template <class T>
	ObservableValue<T>& ObservableValue<T>::operator=(const ObservableValue<T>& t) {
		value_ = t.value_;
		observable_->notifyObservers();
		return *this;
	}

	template <class T>
	ObservableValue<T>::operator T() const {
		return value_;
	}

	template <class T>
	ObservableValue<T>::operator std::shared_ptr<Observable>() const {
		return observable_;
	}

	template <class T>
	const T& ObservableValue<T>::value() const {
		return value_;
	}

}