#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <chrono>

extern "C" {
  typedef struct {void * p;} *array;

  array newarray(uint64_t);
  void setarray(array,uint64_t,uint64_t);
  uint64_t getarray(array,uint64_t);

  void reverse(array, uint64_t, uint64_t);
  array copyArr(array, uint64_t);
}

typedef std::chrono::duration<double> dur;

array q_mk_array(int n) {
  srand(0);
  array a = newarray(n);
  for (size_t i=0;i<n;++i)
    setarray(a,i,rand());

  return a;
}

void q_check(array a, array a_reverse, int first, int last) {
  for (size_t i = 0 ; i+1 < last-first ; ++i) {
    if (getarray(a,first+i) != getarray(a_reverse,last-i-1)) {
      std::cerr<<"q reverse failed"<<std::endl;
      exit(1);
    }
  }
}

dur q_reverse(array a, int n) {
  auto start = std::chrono::system_clock::now();
  reverse(a,0,n);
  auto end = std::chrono::system_clock::now();
  return end-start;
}

void q_check(int n) {

  auto a = q_mk_array(n);
  auto a_reverse = copyArr(a,n);
  auto t = q_reverse(a_reverse,n);
  q_check(a,a_reverse,0,n);
  std::cout<<"Q-reverse took "<<t.count()*1000<<"ms"<<std::endl;
}


typedef uint64_t* carray;

carray c_mk_array(int n) {
  srand(0);
  carray a = new uint64_t[n];
  for (size_t i=0;i<n;++i)
    a[i]=rand();

  return a;
}

carray copyA(carray a, int length){
  carray a_copy = new uint64_t[length];
  for(int i=0; i<length; i++){
    a_copy[i] = a[i];
  }
  return a_copy;
}

void c_check(carray a, carray a_reverse, int first, int last) {
  
  for (size_t i = 0 ; i+1 < last-first ; ++i) {
    if (a[first+i] != a_reverse[last-i-1]) {
      std::cerr<<"cpp reverse failed"<<std::endl;
      exit(1);
    }
  }
  
}

dur c_reverse(carray a, int n) {
  auto start = std::chrono::system_clock::now();
  std::reverse(a+0,a+n);
  auto end = std::chrono::system_clock::now();
  return end-start;
}

void c_check(int n) {
  auto a = c_mk_array(n);
  auto a_reverse = copyA(a,n);
  auto t = c_reverse(a,n);
  c_check(a,a_reverse,0,n);

  std::cout<<"CPP-reverse took "<<t.count()*1000<<"ms"<<std::endl;
}


int main(int argc, char **argv) {

  if (argc!=2)
    for (size_t i=0; i<10000000; i= i?i*2:1 ) q_check(i);
  else {
    if ((std::string)(argv[1]) == "cpp") c_check(10000000);
    else if ((std::string)(argv[1]) == "q") q_check(10000000);
    else {
      std::cerr<<"Arguments are cpp or q"<<std::endl;
      return 1;
    }
  }

 // printf("Success!\n");
  return 0;
}
