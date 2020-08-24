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
  void reverse(array, uint64_t);
}

typedef std::chrono::duration<double> dur;
int count;
int rep;

array q_mk_array() {


  std::ifstream in("tests/benchmark/Java/reverse.txt");
  std::string s;

  getline(in,s);  //count
  count = stoi(s);

  array a = newarray(count);

  getline(in,s);  //rep
  rep = stoi(s);
  
  int i = 0;
  while(i<count && getline(in, s)){
    setarray(a,i++,stoi(s));
  }
  if(i!=count){
    std::cerr << "unenough number" << std::endl;
    exit(1);
  }

  return a;
}

array q_mk_array(int num) {

  array a = newarray(num);

  int i = 0;
  srand(0);
  while(i<num){
    setarray(a,i++,rand());
  }
  
  count = num;
  rep = 1;

  return a;
}

void q_check_reverse(array newArr, array oldArr, uint64_t n) {
  
  uint64_t mid = n/2;
  for(int i=0; i<mid; i++){
    if(getarray(newArr,i) != getarray(oldArr,n-1-i)){
      std::cerr<<"q reverse failed"<<std::endl; 
      exit(1);
    }
  }
}

void q_reverse(array a, uint64_t na) {
  auto start = std::chrono::system_clock::now();
  for(int i=0;i<rep;i++)
    reverse(a,na);
  auto end = std::chrono::system_clock::now();
  std::cout<<"Q-reverse took "<< dur(end-start).count()*1000<<"ms"<<std::endl;
}

void q_check(int n) {
  array a = q_mk_array(n);
  array b = newarray(n);
  for(int i=0; i<n; i++){
    setarray(b,i,getarray(a,i));
  }
  q_reverse(a,n);
  q_check_reverse(a,b,n);
}

void q_check() {
  array a = q_mk_array();
  array b = newarray(count);
  for(int i=0; i<count; i++){
    setarray(b,i,getarray(a,i));
  }
  q_reverse(a,count);
  q_check_reverse(a,b,count);
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
