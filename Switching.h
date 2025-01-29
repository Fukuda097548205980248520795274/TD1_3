#pragma once
#include"GameScene.h"
#include "Enumeration.h"
#include "./Class/Map/Map.h"
#include "./Class/Object/Object.h"


class Switching : virtual public Scene, public Object
{
public:

	Switching();

	Vector2 Easing(const Vector2 a, const Vector2 b, float t);

	void Trigger();


	//void TriangleDraw(Vector2 aPos, Vector2 size, bool isFacingLeft);

	void DrawEasingTriangle(Vector2 center, float baseSize, float t);


	bool isTrigger_;
	bool isSizeUp_;

	Vector2 TrianglePos_;
	float timer_;

};