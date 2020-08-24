#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <chrono>


const size_t RPT = 15000000;

extern "C" {
  typedef struct {void * p;} *array;

  array newarray(uint64_t);
  void setarray(array,uint64_t,uint64_t);
  uint64_t getarray(array,uint64_t);
  void sort(array arr, uint64_t first, uint64_t last);
  bool q_binary_search(array arr, uint64_t first, uint64_t last, uint64_t val);
}

typedef std::chrono::duration<double> dur;

array q_mk_array(int n) {
  srand(0);
  array a = newarray(n);
  for (size_t i=0;i<n;++i)
    setarray(a,i,rand());

  return a;
}

void q_check_search(array a, int n, int val,bool b) {
  bool flag = false;
  for (size_t i=0;i<n;++i) {
    if (getarray(a,i) == val) {
      flag = true;
    }
  }
  if(flag != b){
    std::cerr<<"q search failed"<<std::endl;
    exit(1);
  }
}

bool q_search(array a, int n, int val) {
  bool b;
  auto start = std::chrono::system_clock::now();
  
  for (size_t rpt=0;rpt<RPT;++rpt) 
    b = q_binary_search(a, 0, n, val);
    
  auto end = std::chrono::system_clock::now();

  std::cout<<"Q-search took "<<dur(end-start).count()*1000<<"ms"<<std::endl;
  return b;
}

void q_check(int n) {

  auto a = q_mk_array(n); 

  srand(0);
  uint64_t val = rand();

  sort(a,0,n);
  bool b = q_search(a,n,val);
  q_check_search(a,n,val,b);
 
}


typedef uint64_t* carray;

carray c_mk_array(int n) {
  srand(0);
  carray a = new uint64_t[n];
  for (size_t i=0;i<n;++i)
    a[i]=rand();

  return a;
}

void c_check_search(carray a, int n, int val, bool b) {
  bool flag = false;
  for (size_t i=0;i<n;++i) {
    if (a[i] == val) {
      flag = true;
    }
  }
  if(flag != b){
    std::cerr<<"q search failed"<<std::endl;
    exit(1);
  }

}

bool c_search(carray a, int n, int val) {
  bool b;
  auto start = std::chrono::system_clock::now();

  for (size_t rpt=0;rpt<RPT;++rpt) 
    b = std::binary_search(a+0,a+n,val);

  auto end = std::chrono::system_clock::now();
  std::cout<<"C-search took "<<dur(end-start).count()*1000<<"ms"<<std::endl;
  return b;
}

void c_check(int n) {
  auto a = c_mk_array(n);
  
  srand(0);
  uint64_t val = rand();
  std::sort(a+0,a+n);

  bool b = c_search(a,n,val);
  c_check_search(a,n,val,b);

}


int main(int argc, char **argv) {

  size_t n = 10000000;

  if (argc!=2)
    for (size_t i=0; i<n; i= i?i*2:1 ) q_check(i);
  else {
    if ((std::string)(argv[1]) == "cpp") c_check(n);
    else if ((std::string)(argv[1]) == "q") q_check(n);
    else {
      std::cerr<<"Arguments are cpp or q"<<std::endl;
      return 1;
    }
  }

  //printf("Success!\n");
  return 0;
}
