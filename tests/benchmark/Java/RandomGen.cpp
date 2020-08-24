#include  <iostream>
#include <fstream>    // 读写文件的头文件
#include <string>

using namespace std;

int main(int argSize, char **args) {

    if(argSize!= 4)
    {
        std::cerr<<"must has three parameters "<< argSize <<std::endl;
        exit(1); 
    }
    ofstream ofs;
    string fileN = args[1];
    
    string fileName = "./tests/benchmark/Java/" + fileN + ".txt";
   // std::cout << fileName <<std::endl;
    
    ofs.open(fileName, ios::out);

    string countS = args[2];
    size_t count = stoi(countS);
    string repS = args[3];
    size_t rep = stoi(repS);

    ofs << count << endl;
    ofs << rep << endl;

    if(fileN == "rotate"){
        size_t rotateLocal = rand()%count;
        ofs << rotateLocal << endl;
    }else if(fileN == "binarySearch"){
        size_t checkedNum = rand()%count;
        ofs << checkedNum << endl;
    }else if(fileN == "disjoint"){
        count = count * 2; 
    }

    srand(0); //seed
    for(int i=0; i<count; i++){
        size_t randn = rand();
        ofs << randn << endl;
    }
 
    ofs.close();
    return 0;
}