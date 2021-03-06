//
//  light source.hpp
//  Silver Lobster
//
//  Created by Indi Kernick on 29/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#ifndef light_source_hpp
#define light_source_hpp

#include "reflection.hpp"

struct LightSource {
  int radius;
};

REFL(LightSource,
  REFL_DATA(radius)
);

#endif
