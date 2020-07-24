#include "../header/Parser.h"


int main(int argc, char **argv){

    if (argc < 3) {
      std::cerr<<"Usage [CheckLevelFlag]+ <filename>"<<std::endl;
      std::cerr<<"CheckLevelFlag: DyCheck | notDyCheck | check_arith | check_free | check_array_bound"<<std::endl;
      exit(1);
    }

    try {

      int commandCount = 1;
      while(argv[commandCount+1]!=NULL){
        initCheck(argv[commandCount++]);
      }
      // isCheckLevelValid();

      // if no command, it means they do not need dynamic check
      std::string fileName = argv[commandCount];  // "validCode.q";
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

  return 0;
}
