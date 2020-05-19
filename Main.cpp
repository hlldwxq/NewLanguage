#include "main.h"

int main(){
    std::string fileName = "file.k";
	std::ifstream in(fileName);
    std::ostringstream tmp;
    tmp << in.rdbuf();
    fileStr = tmp.str();

    HandleTopLevel();
}