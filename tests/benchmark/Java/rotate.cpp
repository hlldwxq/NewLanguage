#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <string>

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
  void rotate(array, uint64_t, uint64_t);
}

typedef std::chrono::duration<double> dur;
int count;
int rep;
int rotateLocation;



array q_mk_array() {
  
  
  std::ifstream in("tests/benchmark/Java/rotate.txt");
  std::string s;

  getline(in,s);  //count
  count = stoi(s);

  array a = newarray(count);

  getline(in,s);  //rep
  rep = stoi(s);
  
  getline(in,s);  //rotateLocal
  rotateLocation = stoi(s);

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
  rotateLocation = rand();

  return a;
}

void q_check_rotate(array newArr, array oldArr,uint64_t n) {

  for(int i=0 ; i < n; i++){
    if(i < rotateLocation){
      if( getarray(newArr,i) != getarray(oldArr,i+(n-rotateLocation))){
          std::cerr<<i <<" "<<rotateLocation<<"  q rotate failed1"<<std::endl; 
          //break;
          exit(1);
      }
    }else{
      if( getarray(newArr,i) != getarray(oldArr,i-rotateLocation)){
          std::cerr<<"q rotate failed2"<<std::endl; 
          //break;
          exit(1);
      }
    }
  }
}

void q_rotate(array a, uint64_t na) {

  auto start = std::chrono::system_clock::now();

  for(int i=0;i<rep;i++){
    if(i%2==0)
      rotate(a,rotateLocation,na);
    else
      rotate(a,na-rotateLocation,na);
  }

  auto end = std::chrono::system_clock::now();
  std::cout<<"Q-rotate took "<< dur(end-start).count()*1000<<"ms"<<std::endl;

}

void q_check(int n) {
  array a = q_mk_array(n);
  array b = newarray(n);
  for(int i=0; i<n; i++){
    setarray(b,i,getarray(a,i));
  }

  q_rotate(a,n);
  q_check_rotate(a,b,n);
}

void q_check() {
  array a = q_mk_array();
  array b = newarray(count);
  for(int i=0; i<count; i++){
    setarray(b,i,getarray(a,i));
  }

  q_rotate(a,count);
  q_check_rotate(a,b,count);
}

int main(int argc, char **argv) {

  if (argc!=2){
    for (size_t i=1; i<count; i= i*2 ) 
      q_check(i);
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
