#pragma once
#define _USE_MATH_DEFINES
#include<math.h>
#include "Structure.h"
#include "./Class/Object/Object.h"

class Easing :public Object
{
public:

	static float easedT_;

	static float time_;

	static float endX_;

	static float endY_;

	static void EaseIn(Vec2 pos, int num);

	static void EaseOut(Vec2 pos, int num);

};

