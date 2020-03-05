#pragma once
namespace JSLib {

    template <class T>
    class Disposable : public T {
    public:
        Disposable(T& t);
        Disposable(const Disposable<T>& t);
        Disposable<T>& operator=(const Disposable<T>& t);
    };

    template <class T>
    inline Disposable<T>::Disposable(T& t)
    {
        this->swap(t);
    }

    template <class T>
    inline Disposable<T>::Disposable(const Disposable<T>& t) : T()
    {
        this->swap(const_cast<Disposable<T>&>(t));
    }

    template <class T>
    inline Disposable<T>& Disposable<T>::operator=(const Disposable<T>& t)
    {
        this->swap(const_cast<Disposable<T>&>(t));
        return *this;
    }

}