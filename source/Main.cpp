#include "../header/Parser.h"


int main(int argc, char **argv){

    if (argc != 3) {
      std::cerr<<"Usage llvmir DyCheck <filename> or Usage llvmir notDyCheck <filename>"<<std::endl;
      exit(1);
    }

    try {
      std::string doCheck = argv[1]; // DyCheck notDyCheck
      initCheck(doCheck);

      std::string fileName = argv[2];  // "validCode.q";
      std::ifstream in(fileName);
      std::ostringstream tmp;
      tmp.str("");
      tmp << in.rdbuf();
      in.close();
      std::string ss(tmp.str());                     

      Parser parser = Parser(fileName , ss);
      parser.Parse();
      getModule()->print(outs(), nullptr);

    } catch (std::exception &e) {

      std::cerr<<"ERROR"<<std::endl;
      std::cerr<<e.what()<<std::endl;
      return 1;

    };


}
