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

  void sort(array, uint64_t, uint64_t, bool);
}

typedef std::chrono::duration<double> dur;
int count = 100000;
int rep = 100;
array q_mk_array(int n) {
  srand(0);
  array a = newarray(n);
  for (size_t i=0;i<n;++i){
    int rnd = rand();
    setarray(a,i,rnd);
  }

  return a;
}

void q_check_sorted(array a, int n) {

  for (size_t i=0;i+1<n;++i) {
    if (getarray(a,i) > getarray(a,i+1)) {
      std::cout<<getarray(a,i) <<" "<<getarray(a,i+1)<<std::endl;
      std::cerr<<"q introsort failed"<<std::endl;
      exit(1);
    }
  }
}

dur q_sort(array a, int n) {
  auto start = std::chrono::system_clock::now();
  for(int i=0;i<rep;i++)
    sort(a,0,n,true);
  auto end = std::chrono::system_clock::now();
  return end-start;
}

void q_check(int n) {

  auto a = q_mk_array(n);
  auto t = q_sort(a,n);
  //q_check_sorted(a,n);
  std::cout<<"Q-introsort took "<<t.count()*1000<<"ms"<<std::endl;
}

int main(int argc, char **argv) {

  if (argc!=2){
    for (size_t i=1; i<count; i= i*2 ) 
    {
      std::cout<<i<<"  ";
      q_check(i);
    }
  }
  else {
    if ((std::string)(argv[1]) == "q") q_check(count);
    else {
      std::cerr<<"Arguments should be q"<<std::endl;
      return 1;
    }
  }

 // printf("Success!\n");
  return 0;
}
