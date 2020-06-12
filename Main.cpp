#include "Parser.h"

int main(int argc, char **argv){

    if (argc != 2) {
      std::cerr<<"Usage llvmir <filename>"<<std::endl;
      exit(1);
    }

    try {

      std::string fileName = argv[1];  // "validCode.q";
      std::ifstream in(fileName);
      std::ostringstream tmp;
      tmp << in.rdbuf();
      TheModule = std::make_unique<Module>(fileName, TheContext);
      //fprintf(stderr, " -- %s\n",tmp.str().c_str());
      Parser parser = Parser(tmp.str());
      parser.Parse();
      TheModule->print(outs(), nullptr);
    } catch (std::exception &e) {
      std::cerr<<"ERROR"<<std::endl;
      std::cerr<<e.what()<<std::endl;
      return 1;
    };


}
