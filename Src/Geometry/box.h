#pragma once
#include "kDOP.h"
#include "aabb.h"

#ifdef KDOP
#define BOX kDOP18
#else
#define BOX aabb
#endif
