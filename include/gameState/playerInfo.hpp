#pragma once

struct PlayerInfoObject_Property {
  std::string name;
  std::string value;
  std::string signature;
};

struct PlayerInfoObject {
  UUID uuid;
  std::vector<PlayerInfoObject_Property> properties;
};
