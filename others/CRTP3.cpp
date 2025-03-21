#include <iostream>
#include <vector>

using std::cout; using std::endl;
using std::vector;

class Animal {
 public:
    virtual void say () const = 0;
    virtual ~Animal() {}
};

template <typename T>
class Animal_CRTP: public Animal {
 public:
    void say() const override {
        static_cast<const T*>(this)->say();
    }
};

class Cat: public Animal_CRTP<Cat> {
 public:
    void say() const {
        cout << "Meow~ I'm a cat." << endl;
    }
};

class Dog: public Animal_CRTP<Dog> {
 public:
    void say() const {
        cout << "Wang~ I'm a dog." << endl;
    }
};

int main () {
  vector<Animal*> zoo;
  zoo.push_back(new Cat());
  zoo.push_back(new Dog());
  for (vector<Animal*>::const_iterator iter{zoo.begin()}; iter != zoo.end(); ++iter) {
      (*iter)->say();
  }
  for (vector<Animal*>::iterator iter{zoo.begin()}; iter != zoo.end(); ++iter) {
      delete (*iter);
  }
    return 0;
}