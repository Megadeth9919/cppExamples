#pragma once

#include "Block.h"
#include "Types.h"
class Operation {
public:
  Block *block = nullptr;

  const unsigned numResults;
  const unsigned numSuccs;
  const unsigned numRegions : 23;
  bool hasOperandStorage : 1;
  unsigned char propertiesStorageSize : 8;

};