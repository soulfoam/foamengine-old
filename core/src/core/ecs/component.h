#pragma once

#include <cglm/cglm.h>
#include "../ext/sugar.h"

#define MASK(m) (1 << m)

typedef enum
{
	COMPONENT_TRANSFORM,
	COMPONENT_VELOCITY,


	COMPONENT_COUNT
} ComponentType;

typedef struct
{
	vec3 pos;
} CTransform;

typedef struct
{
	vec3 pos;
} CVelocity;
