#include "JSONObject.hpp"

JSONObject::JSONObject(std::string inp) {
    std::string key_tmp="";
    std::string value_tmp="";
    int mode = 1;
    int mode_backup = 0;
    for(int i = 2; i < inp.length(); i++) {
        if(i==inp.length()-1) {
            mode=0;
        }
        switch(mode) {
            case 0:
                data[key_tmp]=value_tmp;
                key_tmp="";
                value_tmp="";
                mode=1;
                break;
            case 1:
                if(inp[i]=='\\') {
                    mode_backup=mode;mode=10;
                } else if(inp[i]=='"') {
                    mode=2;
                } else {
                    key_tmp+=inp[i];
                }
                break;
            case 2:
                mode=3;
                break;
            case 3:
                if(inp[i]=='\\') {
                    mode_backup=mode;mode=11;
                } else if(inp[i]==',') {
                    mode=0;
                } else {
                    value_tmp+=inp[i];
                }
                break;
            case 10:
                switch(inp[i]) {
                    default:
                        key_tmp+=inp[i];
                        break;
                }
                mode=mode_backup;
                break;
            case 11:
                switch(inp[i]) {
                    case 'n':
                        value_tmp+="\n";
                        break;
                    default:
                        value_tmp+=inp[i];
                        break;
                }
                mode=mode_backup;
                break;
        }
    }
};

JSONObject::~JSONObject() {
}

bool JSONObject::hasEntry(std::string val) {
    return data.count(val)>0;
}

std::string JSONObject::getEntry(std::string val) {
    if(!hasEntry(val)) {return "";}
    return data[val];
}

std::string JSONObject::operator[](std::string index) {
    return getEntry(index);
}