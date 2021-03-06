//
//  field of view.hpp
//  Silver Lobster
//
//  Created by Indi Kernick on 25/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#ifndef field_of_view_hpp
#define field_of_view_hpp

#include <entt/entity/fwd.hpp>
#include <Graphics/geometry.hpp>

void initializeLight(entt::registry &, gfx::Size);
void updateLight(entt::registry &);
void updateVisibility(entt::registry &);
bool canSee(entt::registry &, gfx::Point, gfx::Point);

#endif
