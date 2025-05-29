#include <gtest/gtest.h>
#include "unique_ptr.h"
#include <utility>
#include <type_traits>

namespace tiny_stl {
namespace test {

class UniquePtrTest : public ::testing::Test {
protected:
    struct TestObj {
        TestObj(int val = 0) : value(val) {}
        ~TestObj() { value = -1; }
        int value;
    };

    static int destructor_count;
    struct TrackedObj {
        ~TrackedObj() { destructor_count++; }
    };

    void SetUp() override {
        destructor_count = 0;
    }
};

int UniquePtrTest::destructor_count = 0;

TEST_F(UniquePtrTest, DefaultConstructor) {
    unique_ptr<TestObj> ptr;
    EXPECT_EQ(ptr.get(), nullptr);
}

TEST_F(UniquePtrTest, PointerConstructor) {
    TestObj* raw = new TestObj(42);
    unique_ptr<TestObj> ptr(raw);
    EXPECT_EQ(ptr.get(), raw);
    EXPECT_EQ(ptr->value, 42);
}

TEST_F(UniquePtrTest, MoveConstructor) {
    unique_ptr<TestObj> ptr1(new TestObj(10));
    unique_ptr<TestObj> ptr2(std::move(ptr1));
    
    EXPECT_EQ(ptr1.get(), nullptr);
    EXPECT_NE(ptr2.get(), nullptr);
    EXPECT_EQ(ptr2->value, 10);
}

TEST_F(UniquePtrTest, MoveAssignment) {
    unique_ptr<TestObj> ptr1(new TestObj(20));
    unique_ptr<TestObj> ptr2;
    ptr2 = std::move(ptr1);
    
    EXPECT_EQ(ptr1.get(), nullptr);
    EXPECT_NE(ptr2.get(), nullptr);
    EXPECT_EQ(ptr2->value, 20);
}

TEST_F(UniquePtrTest, DereferenceOperators) {
    unique_ptr<TestObj> ptr(new TestObj(30));
    EXPECT_EQ((*ptr).value, 30);
    EXPECT_EQ(ptr->value, 30);
}

TEST_F(UniquePtrTest, Reset) {
    TestObj* raw1 = new TestObj(1);
    TestObj* raw2 = new TestObj(2);
    unique_ptr<TestObj> ptr(raw1);
    
    ptr.reset(raw2);
    EXPECT_EQ(ptr.get(), raw2);
    EXPECT_EQ(ptr->value, 2);
    
    ptr.reset();
    EXPECT_EQ(ptr.get(), nullptr);
}

TEST_F(UniquePtrTest, Release) {
    TestObj* raw = new TestObj(50);
    unique_ptr<TestObj> ptr(raw);
    
    TestObj* released = ptr.release();
    EXPECT_EQ(released, raw);
    EXPECT_EQ(ptr.get(), nullptr);
    delete released;
}

TEST_F(UniquePtrTest, Destructor) {
    {
        unique_ptr<TrackedObj> ptr(new TrackedObj);
        EXPECT_EQ(destructor_count, 0);
    }
    EXPECT_EQ(destructor_count, 1);
}

TEST_F(UniquePtrTest, ArraySupport) {
    unique_ptr<TestObj[]> ptr(new TestObj[3]{1, 2, 3});
    EXPECT_EQ(ptr[0].value, 1);
    EXPECT_EQ(ptr[1].value, 2);
    EXPECT_EQ(ptr[2].value, 3);
}


TEST_F(UniquePtrTest, CustomDeleter) {
    static bool custom_deleted;
    struct CustomDeleter {
        void operator()(TestObj* p) {
            delete p;
            custom_deleted = true;
        }
    };
    custom_deleted = false;
    {
        unique_ptr<TestObj, CustomDeleter> ptr(new TestObj);
        EXPECT_FALSE(custom_deleted);
    }
    EXPECT_TRUE(custom_deleted);
}

TEST_F(UniquePtrTest, ConversionToBool) {
    unique_ptr<TestObj> ptr1;
    unique_ptr<TestObj> ptr2(new TestObj);
    
    EXPECT_FALSE(ptr1);
    EXPECT_TRUE(ptr2);
}

TEST_F(UniquePtrTest, PolymorphicBehavior) {
  struct Base {
      virtual int value() const { return 10; }
      virtual ~Base() = default;
  };
  struct Derived : Base {
      int value() const override { return 20; }
  };

  unique_ptr<Base> ptr = make_unique<Derived>();
  EXPECT_EQ(ptr->value(), 20); // 多态调用验证
}

TEST_F(UniquePtrTest, MakeUnique) {
  auto ptr = make_unique<TestObj>(99);
  EXPECT_EQ(ptr->value, 99);
}

TEST_F(UniquePtrTest, DerivedToBaseConversion) {
  struct Base { virtual ~Base() = default; };
  struct Derived : Base {};

  unique_ptr<Derived> derived = make_unique<Derived>();
  unique_ptr<Base> base = std::move(derived); // 测试派生类到基类的转换
  EXPECT_NE(base.get(), nullptr);
  EXPECT_EQ(derived.get(), nullptr);
}

TEST_F(UniquePtrTest, ReferenceCaptureInDeleter) {
  int capture_flag = 0;
  auto deleter = [&capture_flag](TestObj* p) {
      delete p;
      capture_flag = 1;
  };

  {
      unique_ptr<TestObj, decltype(deleter)> ptr(new TestObj, deleter);
      EXPECT_EQ(capture_flag, 0);
  }
  EXPECT_EQ(capture_flag, 1); // 验证删除器捕获的引用生效
}

TEST_F(UniquePtrTest, ObjectLifetimeExtension) {
  struct Animal {
      virtual void speak() = 0;
      virtual ~Animal() = default;
  };

  struct Dog : Animal {
      int age;

      Dog(int age_) : age(age_) {
      }

      virtual void speak() {
          printf("Bark! I'm %d Year Old!\n", age);
      }
  };

  struct Cat : Animal {
      int &age;

      Cat(int &age_) : age(age_) {
      }

      virtual void speak() {
          printf("Meow! I'm %d Year Old!\n", age);
      }
  };
  int ref_age = 5;
  auto speak = [](Animal* a) { a->speak(); };

  unique_ptr<Animal> cat = make_unique<Cat>(ref_age);
  unique_ptr<Animal> dog = make_unique<Dog>(ref_age);

  testing::internal::CaptureStdout();
  speak(cat.get());
  speak(dog.get());
  ref_age++;
  speak(cat.get()); // Cat的引用age应随外部变化
  std::string output = testing::internal::GetCapturedStdout();

  EXPECT_TRUE(output.find("Meow! I'm 5") != std::string::npos);
  EXPECT_TRUE(output.find("Meow! I'm 6") != std::string::npos);
  EXPECT_TRUE(output.find("Bark! I'm 5") != std::string::npos);
}

} // namespace test
} // namespace tiny_stl