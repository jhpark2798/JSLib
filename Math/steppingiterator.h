#pragma once
#include <boost/iterator/iterator_adaptor.hpp>

namespace JSLib {

    template <class Iterator>
    class step_iterator :
        public boost::iterator_adaptor<step_iterator<Iterator>, Iterator> {
    
    private:
        typedef boost::iterator_adaptor<step_iterator<Iterator>, Iterator> super_t;
        long step_;
    
    public:
        step_iterator() {}
        explicit step_iterator(const Iterator& base, size_t step)
            : super_t(base), step_(static_cast<long>(step)) {}
       
        template <class OtherIterator> 
        step_iterator(const step_iterator<OtherIterator>& i,
            typename boost::enable_if_convertible
            <OtherIterator, Iterator>::type* = 0)
            : super_t(i.base()), step_(static_cast<long> (i.step())) {}
        size_t step() const { return static_cast<size_t>(this->step_); }
        
        void increment() {
            std::advance(this->base_reference(), step_);
        }
        void decrement() {
            std::advance(this->base_reference(), -step_);
        }
        void advance(typename super_t::difference_type n) {
            this->base_reference() += n * (this->step_);
        }
        typename super_t::difference_type
            distance_to(const step_iterator& i) const {
            return (i.base() - this->base()) / (this->step_);
        }
    };

    template <class Iterator>
    step_iterator<Iterator> make_step_iterator(Iterator it, size_t step) {
        return step_iterator<Iterator>(it, step);
    }

}

