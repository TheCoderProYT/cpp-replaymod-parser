#include "Protocols.hpp"

std::unordered_map<uint32_t,ProtocolInfo> protocols = {
    {756,{
        "1.17.1",{
            {2,{"LOGIN",{
                {0x2,{"LOGIN SUCCESS",FUNC_756_2_2}},
            }}},
            {3,{"PLAY",{
                {0x21,{"KEEPALIVE",FUNC_756_3_21}},
            }}},
        }
    }},
};