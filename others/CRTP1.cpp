#include <iostream>

using namespace std;

template<typename T>
class Base {
 public:
    void show () const {
        static_cast<const T*>(this)->show();
    }
};

class Derived: public Base<Derived> {
 public:
    void show () const {
        cout << "Shown in Derived class." << endl;
    }
};

int main () {
    Derived d;
    Base<Derived> *b = &d;
    b->show();
    return 0;
}