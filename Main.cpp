#include "Parser.h"

int main(){
    std::string fileName = "validCode.q";
	std::ifstream in(fileName);
    std::ostringstream tmp;
    tmp << in.rdbuf();
    TheModule = std::make_unique<Module>(fileName, TheContext);
    //fprintf(stderr, " -- %s\n",tmp.str().c_str());
    Parser parser = Parser(tmp.str());
    parser.Parse();
    TheModule->print(errs(), nullptr);
}