#include <iostream>
#include <cassert>
class Shape {
  public:
    /// Discriminator for LLVM-style RTTI (dyn_cast<> et al.)
    enum ShapeKind {
        SK_Square,
        SK_SpecialSquare,
        SK_OtherSpecialSquare,
        SK_SomeWhatSpecialSquare,
        SK_LastSuqare,
        SK_Circle,
    };

  private:
    const ShapeKind Kind;

  public:
    ShapeKind getKind() const { return Kind; }

  public:
    Shape(ShapeKind K) : Kind(K) {}
    virtual double computeArea() = 0;
};

class Square : public Shape {
    double SideLength;

  public:
    Square(double S) : Shape(SK_Square), SideLength(S) {}
    double computeArea() override { return SideLength * SideLength; }

    static bool classof(const Shape *S) {
        return S->getKind() >= SK_Square && S->getKind() <= SK_LastSuqare;
    }
};

class Circle : public Shape {
    double Radius;

  public:
    Circle(double R) : Shape(SK_Circle), Radius(R) {}
    double computeArea() override { return 3.14 * Radius * Radius; }

    static bool classof(const Shape *S) { return S->getKind() == SK_Circle; }
};

class SpecialSquare : public Square {};
class OtherSpecialSquare : public Square {};
class SomeWhatSpecialSquare : public Square {};

template <typename To, typename From>
inline bool isa(const From* Val) {
  return To::classof(Val);
}

template <typename To, typename From>
inline To* cast(From* Val) {
  assert(isa<To>(Val) && "cast<Ty>() argument of incompatible type!");
  return static_cast<To*>(Val);
}

template <typename To, typename From>
inline To* dyn_cast(From* Val) {
  return isa<To>(Val) ? static_cast<To*>(Val) : nullptr;
}

int main() {
  printf("[static]class:%s   size:%ld\n", "no typeid", sizeof(Square));
  Shape *pShape = new Square(2);
  if (isa<Square>(pShape)) {
      std::cout << "S is Square with area = " << pShape->computeArea()
                << std::endl;
  }
  if (Square *pSquare = cast<Square>(pShape)) {
      std::cout << "cast succeed, S is Square with area = "
                << pSquare->computeArea() << std::endl;
  }
  if (Circle *Cir = dyn_cast<Circle>(pShape)) {
      std::cout << "This won't happen" << std::endl;
  }
  delete cast<Square>(pShape);

  pShape = new Circle(2);
  if (isa<Square>(pShape)) {
      std::cout << "This won't happen" << std::endl;
  }
  if (Circle *Cir = dyn_cast<Circle>(pShape)) {
      std::cout << "Cir is Circle with area = " << Cir->computeArea()
                << std::endl;
  }
  if (Square *pSquare = cast<Square>(pShape)) {
      std::cout << "Assertion will fail" << std::endl;
  }
}



// class Shape {
//   public:
//     Shape() {}
//     virtual double computeArea() = 0;
// };

// class Square : public Shape {
//     double SideLength;

//   public:
//     Square(double S) : SideLength(S) {}
//     double computeArea() override;
// };

// class Circle : public Shape {
//     double Radius;

//   public:
//     Circle(double R) : Radius(R) {}
//     double computeArea() override;
// };

// class SpecialSquare : public Square {};
// class OtherSpecialSquare : public Square {};
// class SomeWhatSpecialSquare : public Square {};