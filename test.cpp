#include <iostream>
#include <time.h>
using namespace std;

int a[]="0123456789";
int *p=a+3;

int main() {
 cout << p-a;
 return 0;
}
