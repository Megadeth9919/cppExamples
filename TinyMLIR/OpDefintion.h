#pragma once
#include "Operation.h"


class OpState {
public:
  Operation *state;
};

template <typename ConcreteType, template <typename T> class... Traits>
class Op : public OpState, public Traits<ConcreteType>... {
public:
  
};