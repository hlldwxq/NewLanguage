#include <cstdlib>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

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
size_t rep;
size_t count;
size_t checkedNum;

size_t str2st(std::string str){
  size_t x;
  std::stringstream ss;
  ss << str;
  ss >> x;
  return x;
}
array q_mk_array(int num) {
  
  array a = newarray(num);
  int i = 0;
  srand(0);
  while(i<num){
    setarray(a,i++,rand());
  }
  sort(a,0,num);
  
  count = num;
  rep = 1;
  checkedNum = rand();
  
  return a;
}
array q_mk_array() {

  std::filebuf fb;
  if(fb.open("tests/benchmark/Java/binarySearch.txt",std::ios::in)==NULL){
    exit(1);
  }
  // std::ifstream fb("D:/postgraduate/MScproject/NewLanguage/tests/benchmark/Java/binarySearch.txt",std::ios::fb);

  std::istream in(&fb);
  std::string s;

  getline(in,s);  //count
  count = str2st(s);
  
  array a = newarray(count);
  
  getline(in,s);  //rep
  rep = str2st(s);
  
  getline(in,s);  //rchecked num
  checkedNum = str2st(s);

  int i = 0;
  while(i<count && getline(in, s)){
    setarray(a,i++,str2st(s));
  }

  fb.close();

  if(i!=count){
    std::cerr << "unenough number" << std::endl;
    exit(1);
  }

  sort(a,0,count);
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
  uint64_t index;

  auto start = std::chrono::system_clock::now();
  for(int i=0;i<rep;i++){
    index = binarySearch(a,0,n,val);
  }
  auto end = std::chrono::system_clock::now();
  std::cout<<"Q-binarySearch took "<< dur(end-start).count()*1000<<"ms"<<std::endl;
  return index;
}

void q_check(int n) {
  array a = q_mk_array(n);
  uint64_t isHas = q_search(a,n,checkedNum);
  q_check_searched(a,n,checkedNum,isHas);
}

void q_check() {
  array a = q_mk_array();
  size_t n = count;
  uint64_t isHas = q_search(a,n,checkedNum);
  q_check_searched(a,n,checkedNum,isHas);
}

int main(int argc, char **argv) {

  if (argc!=2){
    for (size_t i=1; i<count; i= i*2 ) {
      q_check(i);
    }
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
