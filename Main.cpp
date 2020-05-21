#include "Parser.h"

int main(){
    std::string fileName = "file.k";
	std::ifstream in(fileName);
    std::ostringstream tmp;
    tmp << in.rdbuf();
    //fprintf(stderr, " -- %s\n",tmp.str().c_str());
    Parser parser = Parser(tmp.str());

    parser.Parse();
}