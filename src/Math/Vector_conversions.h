#ifndef VECTOR3_CONVERSIONS
#define VECTOR3_CONVERSIONS

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

Vector2 Vector3_toVector2(Vector3 vec);
Vector2 Vector4_toVector2(Vector4 vec);

Vector3 Vector2_toVector3(Vector2 vec);
Vector3 Vector4_toVector3(Vector4 vec);

Vector4 Vector2_toVector4(Vector2 vec);
Vector4 Vector3_toVector4(Vector3 vec);

#endif