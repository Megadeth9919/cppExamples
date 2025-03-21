#include <cstdio>

class Test {
public:
    int a = 10;
    int &b = a;
};

int main() {

  Test t, *pt = &t;
  printf("sizeof int is %d.\n", sizeof (int));
  printf("sizeof Test is %d.\nsizeof (int *) is %d.\n", sizeof t, sizeof (int *));
  printf("%x---%d\n", pt, *pt);
  int *ptt = (int *)pt;   // 首先拿到对象的地址，并转成int*类型，便于我们偏移
  printf("%x---%x\n", ptt - 2, *(ptt - 2));  // 利用偏移量就可以拿到引用成员变量的地址和存储的值
  return 0;
}