#include "processJSONLine.hpp"

std::unordered_map<std::string,std::string> processJSONLine(std::string inp) {
  std::unordered_map<std::string,std::string> output;
  std::string key_tmp="";
  std::string value_tmp="";
  int a = 1;
  int b = 0;
  for(int i = 2; i < inp.length(); i++) {
    if(i==inp.length()-1) {a=0;}
    switch(a) {
      case 0:
      output[key_tmp]=value_tmp;
      //printf("%s: %s\n",x.c_str(),y.c_str());
      key_tmp="";
      value_tmp="";
      a=1;
      break;
      case 1:
      if(inp[i]=='\\') {b=a;a=10;} else
      if(inp[i]=='"') {a=2;} else {
        key_tmp+=inp[i];
      }
      break;
      case 2:
      a=3;
      break;
      case 3:
      if(inp[i]=='\\') {b=a;a=11;} else
      if(inp[i]==',') {a=0;} else {
        value_tmp+=inp[i];
      }
      break;
      case 10:
      switch(inp[i]) {
        default:
        key_tmp+=inp[i];
        break;
      }
      a=b;
      break;
      case 11:
      switch(inp[i]) {
        default:
        value_tmp+=inp[i];
        break;
      }
      a=b;
      break;
    }
  }
  return output;
}
