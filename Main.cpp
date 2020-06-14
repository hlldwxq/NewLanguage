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
      tmp.str("");
      tmp << in.rdbuf();
      in.close();
     
      std::string ss(tmp.str());

      TheModule = std::make_unique<Module>(fileName, TheContext);
      Parser parser = Parser(ss);

      parser.Parse();

      TheModule->print(outs(), nullptr);

    } catch (std::exception &e) {

      std::cerr<<"ERROR"<<std::endl;
      std::cerr<<e.what()<<std::endl;
      return 1;

    };


}
