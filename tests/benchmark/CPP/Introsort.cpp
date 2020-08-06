#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <chrono>

void prs(char* s){
  printf("%s",s);
}
void prl(){
  printf("\n");
}
extern "C" {
  typedef struct {void * p;} *array;

  array newarray(uint64_t);
  void setarray(array,uint64_t,uint64_t);
  uint64_t getarray(array,uint64_t);

  void sort(array, uint64_t, uint64_t);
}

typedef std::chrono::duration<double> dur;

array q_mk_array(int n) {
  srand(0);
  array a = newarray(n);
  for (size_t i=0;i<n;++i)
    setarray(a,i,rand());

  return a;
}

void q_check_sorted(array a, int n) {
  for (size_t i=0;i+1<n;++i) {
    if (getarray(a,i) > getarray(a,i+1)) {
      std::cerr<<"q introsort failed"<<std::endl;
      exit(1);
    }
  }
}

dur q_sort(array a, int n) {
  auto start = std::chrono::system_clock::now();
  sort(a,0,n);
  auto end = std::chrono::system_clock::now();
  return end-start;
}

void q_check(int n) {

  auto a = q_mk_array(n);
  auto t = q_sort(a,n);
  q_check_sorted(a,n);
  std::cout<<"Q-introsort took "<<t.count()*1000<<"ms"<<std::endl;
}


typedef uint64_t* carray;

carray c_mk_array(int n) {
  srand(0);
  carray a = new uint64_t[n];
  for (size_t i=0;i<n;++i)
    a[i]=rand();

  return a;
}

void c_check_sorted(carray a, int n) {
  for (size_t i=0;i+1<n;++i) {
    if (a[i] > a[i+1]) {
      std::cerr<<"c introsort failed"<<std::endl;
      exit(1);
    }
  }
}

dur c_sort(carray a, int n) {
  auto start = std::chrono::system_clock::now();
  std::sort(a+0,a+n);
  auto end = std::chrono::system_clock::now();
  return end-start;
}

void c_check(int n) {
  auto a = c_mk_array(n);
  auto t = c_sort(a,n);
  c_check_sorted(a,n);

  std::cout<<"C-introsort took "<<t.count()*1000<<"ms"<<std::endl;
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
