#pragma once
#include <boost/core/noncopyable.hpp>
#include <memory>

namespace JSLib{

	template <class T>
	class Singleton : private boost::noncopyable {	// ¿Ö private »ó¼Ó?
	public:
		static T& instance();
	protected:
		Singleton() {}
	private:
		static std::shared_ptr<T> instance_;
	};

	template <class T>
	std::shared_ptr<T> Singleton<T>::instance_ = nullptr;

	template <class T>
	T& Singleton<T>::instance() {
		if (!instance_)
			instance_ = std::shared_ptr<T>(new T);
		return *instance_;
	}

}