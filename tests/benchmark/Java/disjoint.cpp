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
  bool disjoint(array, uint64_t, array, uint64_t);
}

typedef std::chrono::duration<double> dur;
array a;
array b;
int rep = 10;
int count = 10000;
void q_mk_array(int n) {
  a = newarray(n);
  b = newarray(n);
  srand(0);
  for (size_t i=0;i<n;++i){
    setarray(a,i,rand());
    setarray(b,i,rand());
  }
}

void q_check_disjoint(array a, array b, uint64_t n, bool flag) {
  bool test = true;
  for(int i=0; i<n ;i++){
    for(int j=0; j<n ;j++){
      if(getarray(a,i) == getarray(b,j)){
        test = false;
        break;
      }
    }
  }
  if(test != flag){
    std::cerr<<"q disjoint failed"<<std::endl;
    exit(1);
  }
}

bool q_disjoint(array a, uint64_t na, array b, uint64_t nb) {
  auto start = std::chrono::system_clock::now();
  
  bool result;
  for(int i=0;i<rep;i++){
    result = disjoint(a,na,b,nb);
  }

  auto end = std::chrono::system_clock::now();
  std::cout<<"Q-disjoint took "<< dur(end-start).count()*1000<<"ms"<<std::endl;
  return result;
}

void q_check(int n) {
  q_mk_array(n);
  bool flag = q_disjoint(a,n,b,n);
  q_check_disjoint(a,b,n,flag);
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
