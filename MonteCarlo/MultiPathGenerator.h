#pragma once

#include <memory>
#include <functional>

#include "..//Process/StochasticProcess.h"
#include "Sample.h"
#include "../TimeGrid.h"
#include "../Math/Array.h"
#include "Path.h"
#include "MultiPath.h"

namespace JSLib {

	template <class GSG> // GSG : GaussianSequenceGenerator
	class MultiPathGenerator {
	public:
		typedef Sample<MultiPath> sample_type;
		MultiPathGenerator(const std::shared_ptr<StochasticProcess>&,
			const TimeGrid&, GSG generator, bool brownianBridge = false);
		const sample_type& next() const { return next(false); }
		const sample_type& antithetic() const { return next(true); }
	private:
		// 
		const sample_type& next(bool antithetic) const; // antithetic=최근에만들었던거쓸거냐?
		bool brownianBridge_; // 무슨역할?
		std::shared_ptr<StochasticProcess> process_;
		GSG generator_;
		mutable sample_type next_;
	};

	template <class GSG>
	MultiPathGenerator<GSG>::MultiPathGenerator(
		const std::shared_ptr<StochasticProcess>& process,
		const TimeGrid& timeGrid, GSG generator, bool brownianBridge) 
		: brownianBridge_(brownianBridge), process_(process),
		generator_(generator), next_(MultiPath(process->size(), timeGrid), 1.0){
		// 미완성
	}

	template <class GSG>
	const typename MultiPathGenerator<GSG>::sample_type&
		MultiPathGenerator<GSG>::next(bool antithetic) const {
		if (brownianBridge_) {
			throw std::runtime_error("Brownian bridge not supported");
		}
		typedef typename GSG::sample_type sequence_type;
		const sequence_type& sequence_ =
			antithetic ? generator_.lastSequence() : generator_.nextSequence();
		std::size_t m = process_->size(), n = process_->factors();
		MultiPath& path = next_.value;
		Array asset = process_->initialValues();
		for (std::size_t j = 0; j < m; ++j) {
			path[j].front() = asset[j];
		}
		Array temp(n);	// 무슨 역할?
		next_.weight = sequence_.weight;

		TimeGrid timeGrid = path[0].timeGrid();
		double t, dt;
		for (std::size_t i = 1; i < path.pathSize(); i++) {
			std::size_t offset = (i - 1) * n; // 무슨 역할?
			t = timeGrid[i - 1];
			dt = timeGrid.dt(i - 1);
			if (antithetic)
				std::transform(sequence_.value.begin() + offset,
					sequence_.value.begin() + offset + n,
					temp.begin(),
					std::negate<double>());
			else
				std::copy(sequence_.value.begin() + offset,
					sequence_.value.begin() + offset + n,
					temp.begin());
			asset = process_->evolve(t, asset, dt, temp);
			for (std::size_t j = 0; j < m; ++j) {
				path[j][i] = asset[j];
			}
		}
		return next_;
	}

}