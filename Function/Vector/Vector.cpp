#include "Vector.h"

/// <summary>
/// 正規化した値を作る
/// </summary>
/// <param name="vec">ベクトル</param>
/// <returns>正規化した値を返却する</returns>
struct Vec2 Normalize(struct Vec2 vec)
{
	float length = sqrtf(powf(vec.x , 2) + powf(vec.y , 2));

	if (length != 0.0f)
	{
		vec = { vec.x / length , vec.y / length };
	}

	return vec;
}