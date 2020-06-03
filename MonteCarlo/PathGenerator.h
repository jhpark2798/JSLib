#pragma once

#include "Path.h"
#include "..//Process/StochasticProcess.h"
#include "Sample.h"
#include "..//TimeGrid.h"

namespace JSLib{

	template <class GSG>
	class PathGenerator {
	public:
		typedef Sample<Path> sample_type;
		PathGenerator(const std::shared_ptr<StochasticProcess>&,
			const TimeGrid& timeGrid, const GSG& generator,
			bool brownianBridge);
		const sample_type& next() const;
		const sample_type& antithetic() const;
		std::size_t size() const { return dimension_; }
		const TimeGrid& timeGrid() const { return timeGrid_; }
	private:
		const sample_type& next(bool antithetic) const;
		bool brownianBridge_;
		GSG generator_;
		std::size_t dimension_;
		TimeGrid timeGrid_;
		std::shared_ptr<StochasticProcess1D> process_;
		mutable sample_type next_;
		mutable std::vector<double> temp_;
	};

	template <class GSG>
	const typename PathGenerator<GSG>::sample_type&
		PathGenerator<GSG>::next() const {
		return next(false);
	}

	template <class GSG>
	const typename PathGenerator<GSG>::sample_type&
		PathGenerator<GSG>::antithetic() const {
		return next(true);
	}

	template <class GSG>
	PathGenerator<GSG>::PathGenerator(
		const std::shared_ptr<StochasticProcess>& process,
		const TimeGrid& timeGrid, const GSG& generator,
		bool brownianBridge)
		: brownianBridge_(brownianBridge), generator_(generator),
		dimension_(generator_.dimension()), timeGrid_(timeGrid),
		process_(std::dynamic_pointer_cast<StochasticProcess1D>(process)),
		next_(Path(timeGrid_), 1.0), temp_(dimension_) {
		// ¹Ì¿Ï¼º
	}

	template <class GSG>
	const typename PathGenerator<GSG>::sample_type&
		PathGenerator<GSG>::next(bool antithetic) const {
		if (brownianBridge_) {
			throw std::runtime_error("Brownian bridge not supported");
		}
		typedef typename GSG::sample_type sequence_type;
		const sequence_type& sequence_ =
			antithetic ? generator_.lastSequence() : generator_.nextSequence();
		std::copy(sequence_.value.begin(), sequence_.value.end(), temp_.begin());
		next_.weight = sequence_.weight;
		Path& path = next_.value;
		path.front() = process_->x0();
		for (std::size_t i = 1; i < path.length(); ++i) {
			double t = timeGrid_[i - 1];
			double dt = timeGrid_.dt(i - 1);
			path[i] = process_->evolve(t, path[i - 1], dt, antithetic ? -temp_[i - 1] : temp_[i - 1]);
		}
		return next_;
	}

}