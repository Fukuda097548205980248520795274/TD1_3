#include "Easing.h"

float Easing::easedT_ = 0.0f;
float Easing::time_ = 0.0f;
float Easing::endX_ = 0.0f;
float Easing::endY_ = 0.0f;

void Easing::EaseIn(Vec2 pos, int num)
{
	time_ += 0.01f;
	easedT_ = time_ * time_;
	pos.x = (1.0f - easedT_) * pos.x + easedT_ * endX_;

	if (num == 0)
	{
		if (pos.x >= endX_)
		{
			pos.x = endX_;
		}
	}
	else if (num == 1)
	{
		if (pos.x <= endX_)
		{
			pos.x = endX_;
		}
	}
}

void Easing::EaseOut(Vec2 pos, int num)
{
	time_ += 0.01f;
	easedT_ = 1.0f - static_cast<float>(pow(1.0f - time_, 3.0f));

	pos.x = (1.0f - easedT_) * pos.x + easedT_ * endX_;

	if (num == 0)
	{
		if (pos.x >= endX_)
		{
			pos.x = endX_;
		}
	}
	else if (num == 1)
	{
		if (pos.x <= endX_)
		{
			pos.x = endX_;
		}
	}
}