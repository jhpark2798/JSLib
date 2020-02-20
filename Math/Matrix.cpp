#include "Matrix.h"
#include <boost/serialization/array_wrapper.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/lu.hpp>

namespace JSLib {

    Disposable<Matrix> inverse(const Matrix& m) {
        boost::numeric::ublas::matrix<double> a(m.rows(), m.columns());
        std::copy(m.begin(), m.end(), a.data().begin());

        boost::numeric::ublas::permutation_matrix<size_t> pert(m.rows());
      
        size_t singular = lu_factorize(a, pert);
    
        if (singular != 0)
            throw std::domain_error("The input matrix isn't invertible.");

        boost::numeric::ublas::matrix<double> inverse = boost::numeric::ublas::identity_matrix<double>(m.rows());
        
        boost::numeric::ublas::lu_substitute(a, pert, inverse);
 
        Matrix retVal(m.rows(), m.columns());
        std::copy(inverse.data().begin(), inverse.data().end(), retVal.begin());

        return retVal;
    }

	double determinant(const Matrix& m)
	{
        boost::numeric::ublas::matrix<double> a(m.rows(), m.columns());
        std::copy(m.begin(), m.end(), a.data().begin());

        boost::numeric::ublas::permutation_matrix<size_t> pert(m.rows());
        lu_factorize(a, pert);

        double retVal = 1.0;

        for (size_t i = 0; i < m.rows(); ++i) {
            if (pert[i] != i)
                retVal *= -a(i, i);
            else
                retVal *= a(i, i);
        }
        return retVal;
	}
}