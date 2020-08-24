#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
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
int rep;
int count;

void q_mk_array() {

  std::ifstream in("tests/benchmark/Java/disjoint.txt");
  std::string s;

  getline(in,s);  //count
  count = stoi(s);

  getline(in,s);  //rep
  rep = stoi(s);
  

  int i = 0;
  
  a = newarray(count);
  b = newarray(count);
  while(i<count && getline(in, s)){
    setarray(a,i,stoi(s));
    i++;
  }
  if(i!=count){
    std::cerr << "unenough number" << std::endl;
    exit(1);
  }

  i = 0;
  while(i<count && getline(in, s)){
    setarray(b,i,stoi(s));
    i++;
  }
  if(i!=count){
    std::cerr << "unenough number" << std::endl;
    exit(1);
  }

}

void q_mk_array(int num) {

  a = newarray(num);
  b = newarray(num);

  int i = 0;
  srand(0);
  while(i<num){
    setarray(a,i,rand());
    setarray(a,i++,rand());
  }
  
  count = num;
  rep = 1;

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
  bool result;
  auto start = std::chrono::system_clock::now();
  
  for(int i=0;i<rep;i++){
    result = disjoint(a,na,b,nb);
  }

  auto end = std::chrono::system_clock::now();
  std::cout<<"Q-disjoint took "<< dur(end-start).count()*1000<<"ms"<<std::endl;
  return result;
}

void q_check() {

  q_mk_array();
  bool flag = q_disjoint(a,count,b,count);
  q_check_disjoint(a,b,count,flag);
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
    if ((std::string)(argv[1]) == "q") q_check();
    else {
      std::cerr<<"Arguments should be q"<<std::endl;
      return 1;
    }
  }

  return 0;
}
