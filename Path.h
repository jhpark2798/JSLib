#pragma once
#include "../TimeGrid.h"
#include "../Math/Array.h"

namespace JSLib {

	class Path {
	public:
		Path(const TimeGrid& timeGrid, const Array& values = Array());
		
		bool empty() const;
		size_t length() const;

		double operator[](size_t i) const;
		double at(size_t i) const;
		double& operator[](size_t i);
		double& at(size_t i);
		double value(size_t i) const;
		double& value(size_t i);

		double time(size_t i) const;

		double front() const;
		double& front();
		double back() const;
		double& back();

		const TimeGrid& timeGrid() const;

		typedef Array::const_iterator iterator;
		typedef Array::const_reverse_iterator reverse_iterator;
		iterator begin() const;
		iterator end() const;
		reverse_iterator rbegin() const;
		reverse_iterator rend() const;

	private:
		TimeGrid timeGrid_;
		Array values_;
	};

	inline Path::Path(const TimeGrid& timeGrid, const Array& values) : timeGrid_(timeGrid), values_(values)
	{
		if (values_.empty())
			values_ = Array(timeGrid_.size());
		JS_REQUIRE(values_.size() == timeGrid_.size(), "different number of times and asset values");
	}

	inline bool Path::empty() const
	{
		return timeGrid_.empty();
	}

	inline size_t Path::length() const
	{
		return timeGrid_.size();
	}

	inline double Path::operator[](size_t i) const
	{
		return values_[i];
	}

	inline double Path::at(size_t i) const
	{
		return values_.at(i);
	}

	inline double& Path::operator[](size_t i)
	{
		return values_[i];
	}

	inline double& Path::at(size_t i)
	{
		return values_.at(i);
	}

	inline double Path::value(size_t i) const
	{
		return values_[i];
	}

	inline double& Path::value(size_t i)
	{
		return values_[i];
	}

	inline double Path::front() const
	{
		return values_[0];
	}

	inline double& Path::front()
	{
		return values_[0];
	}

	inline double Path::back() const
	{
		return values_[values_.size() - 1];
	}

	inline double& Path::back()
	{
		return values_[values_.size() - 1];
	}

	inline double Path::time(size_t i) const
	{
		return timeGrid_[i];
	}

	inline const TimeGrid& Path::timeGrid() const
	{
		return timeGrid_;
	}

	inline Path::iterator Path::begin() const
	{
		return values_.begin();
	}

	inline Path::iterator Path::end() const
	{
		return values_.end();
	}

	inline Path::reverse_iterator Path::rbegin() const
	{
		return values_.rbegin();
	}

	inline Path::reverse_iterator Path::rend() const
	{
		return values_.rend();
	}

}


