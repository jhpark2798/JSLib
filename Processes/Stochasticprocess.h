#pragma once
#include "../Time/Date.h"
#include "../DesignPattern/Observer.h"
#include "../Math/Matrix.h"

namespace JSLib {

	//discretization를 내부 클래스로 정의하는 이유
	//1D process에 동일한 함수를 반환 타입만 다르게 해서 새로 정의하는 이유

	//discretization drift -> 주어진 discretization 방법에 따른 dt 이후 drift term을 구해줌
	//discretization diffusion -> 주어진 discretization 방법에 따른 dt 이후 diffusion term을 구해줌

	class StochasticProcess : public Observer, Observable {
	public:
		class discretization {
		public:
			virtual ~discretization() {}
			virtual Disposable<Array> drift(const StochasticProcess&, double t0, const Array& x0, double dt) const = 0; 
			virtual Disposable<Matrix> diffusion(const StochasticProcess&, double t0, const Array& x0, double dt) const = 0;
			virtual Disposable<Matrix> covariance(const StochasticProcess&, double t0, const Array& x0, double dt) const = 0;
		};

		virtual ~StochasticProcess() {}
		virtual size_t size() const = 0;
		virtual size_t factors() const;
		virtual Disposable<Array> initialValues() const = 0;
		virtual Disposable<Array> drift(double t, const Array& x) const = 0;
		virtual Disposable<Matrix> diffusion(double t, const Array& x) const = 0;
		virtual Disposable<Array> expectation(double t0, const Array& x0, double dt) const;//t0에서 dt 사이의 process의 expectation
		virtual Disposable<Matrix> stdDeviation(double t0, const Array& x0, double dt) const;//t0에서 dt 사이의 process의 std
		virtual Disposable<Matrix> covariance(double t0, const Array& x0, double dt) const;//t0에서 dt 사이의 process의 covariance
		virtual Disposable<Array> evolve(double t0, const Array& x0, double dt, const Array& dw) const;//t0에서 dt 경과 후 process의 value
		virtual Disposable<Array> apply(const Array& x0, const Array& dx) const;//process의 x0에 dx를 더한 값을 반환

		virtual double time(const Date&) const;

		void update();

	protected:
		StochasticProcess();
		explicit StochasticProcess(const std::shared_ptr<discretization>&);
		std::shared_ptr<discretization> discretization_;
	};

	class StochasticProcess1D : public StochasticProcess {
	public:
		class discretization {
		public:
			virtual ~discretization() {}
			virtual double drift(const StochasticProcess1D&, double t0, double x0, double dt) const = 0;
			virtual double diffusion(const StochasticProcess1D&, double t0, double x0, double dt) const = 0;
			virtual double variance(const StochasticProcess1D&, double t0, double x0, double dt) const = 0;
		};

		virtual double x0() const = 0;
		virtual double drift(double t, double x) const = 0;
		virtual double diffusion(double t, double x) const = 0;
		virtual double expectation(double t0, double x0, double dt) const;
		virtual double stdDeviation(double t0, double x0, double dt) const;
		virtual double variance(double t0, double x0, double dt) const;
		virtual double evolve(double t0, double x0, double dt, double dw) const;
		virtual double apply(double x0, double dx) const;

	protected:
		StochasticProcess1D();
		explicit StochasticProcess1D(const std::shared_ptr<discretization>&);
		std::shared_ptr<discretization> discretization_;

	private:
		size_t size() const;
		Disposable<Array> initialValues() const;
		Disposable<Array> drift(double t, const Array& x) const;
		Disposable<Matrix> diffusion(double t, const Array& x) const;
		Disposable<Array> expectation(double t0, const Array& x0, double dt) const;
		Disposable<Matrix> stdDeviation(double t0, const Array& x0, double dt) const;
		Disposable<Matrix> covariance(double t0, const Array& x0, double dt) const;
		Disposable<Array> evolve(double t0, const Array& x0, double dt, const Array& dw) const;
		Disposable<Array> apply(const Array& x0, const Array& dx) const;
	};

	inline size_t StochasticProcess1D::size() const
	{
		return 1;
	}

	inline Disposable<Array> StochasticProcess1D::initialValues() const
	{
		Array a(1, x0());
		return a;
	}

	inline Disposable<Array> StochasticProcess1D::drift(double t, const Array& x) const
	{
		Array a(1, drift(t, x[0]));
		return a;
	}

	inline Disposable<Matrix> StochasticProcess1D::diffusion(double t, const Array& x) const
	{
		Matrix m(1, 1, diffusion(t, x[0]));
		return m;
	}

	inline Disposable<Array> StochasticProcess1D::expectation(double t0, const Array& x0, double dt) const
	{
		Array a(1, expectation(t0, x0[0], dt));
		return a;
	}

	inline Disposable<Matrix> StochasticProcess1D::stdDeviation(double t0, const Array& x0, double dt) const
	{
		Matrix m(1, 1, stdDeviation(t0, x0[0], dt));
		return m;
	}

	inline Disposable<Matrix> StochasticProcess1D::covariance(double t0, const Array& x0, double dt) const
	{
		Matrix m(1, 1, variance(t0, x0[0], dt));
		return m;
	}

	inline Disposable<Array> StochasticProcess1D::evolve(double t0, const Array& x0, double dt, const Array& dw) const
	{
		Array a(1, evolve(t0, x0[0], dt, dw[0]));
		return a;
	}

	inline Disposable<Array> StochasticProcess1D::apply(const Array& x0, const Array& dx) const
	{
		Array a(1, apply(x0[0], dx[0]));
		return a;
	}


}