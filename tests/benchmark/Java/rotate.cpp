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
  void rotate(array, uint64_t, uint64_t);
}

typedef std::chrono::duration<double> dur;
int count = 1000000;
int rep = 101;

array q_mk_array(int n) {
  array a = newarray(n);
  srand(0);
  for (size_t i=0;i<n;++i){
    setarray(a,i,rand());
  }
  return a;
}

void q_check_rotate(array newArr, array oldArr, uint64_t rotateLocation,uint64_t n) {
  rotateLocation = rotateLocation%n;
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

void q_rotate(array a, uint64_t distance, uint64_t na) {
  auto start = std::chrono::system_clock::now();
  for(int i=0;i<rep;i++){
    if(i%2==0)
      rotate(a,distance,na);
    else
      rotate(a,na-distance,na);
    
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

  int distance = rand()%n;
  
  q_rotate(a,distance,n);
  q_check_rotate(a,b,distance,n);
}

int main(int argc, char **argv) {

  if (argc!=2){
    for (size_t i=1; i<count; i= i*2 ) 
      q_check(i);
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
