#pragma once

#include <vector>


std::unordered_map<uint32_t,ProtocolInfo> protocols = {
  {756,{"1.17.1",{
    {2,{"LOGIN",{
      {0x02,{"Login Success",LOGIN_LOGINSUCCESS}},
    }}},
    {3,{"PLAY",{
      {0x36,{"Player Info",PLAY_PLAYERINFO}},
    }}},

  }}},
};
