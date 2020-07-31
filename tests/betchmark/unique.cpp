#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <chrono>

extern "C" {
  typedef struct {void * p;} *array;

  array newarray(uint64_t);
  void setarray(array,uint64_t,uint64_t);
  uint64_t getarray(array,uint64_t);
  void sort(array arr, uint64_t first, uint64_t last);
  uint64_t unique(array, uint64_t, uint64_t);
}

typedef std::chrono::duration<double> dur;

array q_mk_array(int n) {
  srand(0);
  array a = newarray(n);
  for (size_t i=0;i<n;++i)
    setarray(a,i,rand());

  return a;
}

void q_check(array a,int first, int last) {
  for (size_t i = 1 ; i+1 < last-first ; ++i) {
    if (getarray(a,first+i-1) == getarray(a,first+i)) {
      std::cerr<<"q reverse failed"<<std::endl;
      exit(1);
    }
  }
}

int q_unique(array a, int n) {
  auto start = std::chrono::system_clock::now();
  int finalN = unique(a,0,n);
  auto end = std::chrono::system_clock::now();
  std::cout<<"Q-uinque took "<<dur(end-start).count()*1000<<"ms"<<std::endl;
  return finalN;
}

void q_check(int n) {

  auto a = q_mk_array(n);
  sort(a,0,n);
  int finalN = q_unique(a,n);
  q_check(a,0,finalN);

}


typedef uint64_t* carray;

carray c_mk_array(int n) {
  srand(0);
  carray a = new uint64_t[n];
  for (size_t i=0;i<n;++i)
    a[i]=rand();

  return a;
}


void c_check(carray a,int first, int last) {
  
  for (size_t i = 0 ; i < last-first-2 ; ++i) {
    if (a[first+i] == a[first+i+1]) {
      std::cerr<<"cpp unique failed"<<std::endl;
      exit(1);
    }
  }
  
}

int c_unique(carray a, int n) {
  auto start = std::chrono::system_clock::now();
  int finalNum = std::unique(a+0,a+n) - a;
  auto end = std::chrono::system_clock::now();
  std::cout<<"CPP-unique took "<<dur(end-start).count()*1000<<"ms"<<std::endl;
  return finalNum;
}

void c_check(int n) {
  auto a = c_mk_array(n);
  std::sort(a+0,a+n);
  int finalNum = c_unique(a,n);
  c_check(a,0,finalNum);
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
