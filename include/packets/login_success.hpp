#pragma once

void LOGIN_LOGINSUCCESS() {
  UUID playerUUID = readUUID();
  std::string playerString = readString();
  fprintf(fileOutput,"Login Success for \"%s\" (%s)\n",playerString.c_str(),playerUUID.toString());
  connectionState=3;
}
