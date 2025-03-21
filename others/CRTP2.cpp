#include <iostream>

using namespace std;

template<typename T>
class Base {
 public:
    void show () const {
        static_cast<const T*>(this)->show();
    }
    Base<T> operator++() {
        static_cast<T*>(this)->operator++();
    }
};

class Derived: public Base<Derived> {
 public:
    Derived () : val(0) {};
    void show () const {
        cout << "Shown in Derived class." << endl;
        cout << "Val is " << this->val << "." << endl;
    }
    Derived operator++() {
        ++(this->val);
        return *this;
    }
 private:
    int val;
};

int main () {
    Derived d;
    Base<Derived> *b = &d;
    b->show();
    ++(*b);
    b->show();
    return 0;
}