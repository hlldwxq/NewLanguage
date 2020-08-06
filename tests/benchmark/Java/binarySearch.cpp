#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <chrono>

void prs(char* s){
  printf("%s",s);
}
void pr(int n){
  printf("%d",n);
}
void prl(){
  printf("\n");
}
extern "C" {
  typedef struct {void * p;} *array;
  array newarray(uint64_t);
  void setarray(array,uint64_t,uint64_t);
  uint64_t getarray(array,uint64_t);
  void sort(array arr, uint64_t first, uint64_t last);
  uint64_t binarySearch(array, uint64_t, uint64_t, uint64_t);
}

typedef std::chrono::duration<double> dur;
int rep = 10000;
int count = 10000000;
array q_mk_array(int n) {
  srand(0);
  array a = newarray(n);
  for (size_t i=0;i<n;++i){
    setarray(a,i,rand());
  }
  sort(a,0,n);
  return a;
}

void q_check_searched(array a, uint64_t n, uint64_t val, uint64_t index) {
  uint64_t test = n;

  uint64_t low = 0;
  uint64_t high = n - 1;
  while (low <= high) {
    uint64_t mid = (low + high) /2 ;
    uint64_t midVal = getarray(a,mid);

    if (midVal < val) { low = mid + 1; }
    else{
        if (midVal > val) { high = mid - 1; }
        else { test = mid; break;}   // key found
    }
  }

  if(test!=index ){
    std::cerr<<"q search failed"<<std::endl;
    exit(1);
  }
}

uint64_t q_search(array a, uint64_t n, uint64_t val) {
  auto start = std::chrono::system_clock::now();
  uint64_t index;
  for(int i=0;i<rep;i++){
    index = binarySearch(a,0,n,val);
  }
  auto end = std::chrono::system_clock::now();
  std::cout<<"Q-binarySort took "<< dur(end-start).count()*1000<<"ms"<<std::endl;
  return index;
}

void q_check(int n) {
  auto a = q_mk_array(n);
  srand(0);
  uint64_t val = rand();
  uint64_t isHas = q_search(a,n,val);
  q_check_searched(a,n,val,isHas);
  
}

int main(int argc, char **argv) {

  if (argc!=2){
    for (size_t i=1; i<count; i= i*2 ) q_check(i);
  }
  else {
    if ((std::string)(argv[1]) == "q") q_check(count);
    else {
      std::cerr<<"Arguments should be q"<<std::endl;
      return 1;
    }
  }

  return 0;
}
