#include "fail.h"

struct A {
    template< class T >
    struct B {
        B() {
            val_ = 0;
        }

        template< class U >
        B( U u ) {
            val_ = 2 * u;
        }

        T f();

        static T g(T t) {
            return 3 * t;
        }

    private:
        T val_;
    };

public:
    template< class T >
    int f( T t ) {
        return 2 * t;
    }
};

template<class T>
T A::B<T>::f() {
    return val_;
}


template< class T >
struct C {
    template< class U >
    struct B {
    };

    template< class U >
    T f( U u ) {
        return 2 * u + 1;
    }
};


int main() {
    A::B<int> b1;
    if( b1.f() != 0 ) fail( __LINE__ );

    A::B<int> b2(1);
    if( b2.f() != 2 ) fail( __LINE__ );

    static short val = 2;
    A::B<int> b3(val);
    if( b3.f() != 4 ) fail( __LINE__ );

    if( A::B<int>::g(2) != 6 ) fail( __LINE__ );

    C<int>::B<int> cb;

    C<int> c;
    if( c.f( 2 ) != 5 ) fail( __LINE__ );


    _PASS;
}
