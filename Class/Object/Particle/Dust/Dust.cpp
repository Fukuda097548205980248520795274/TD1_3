#include"./Class/Object/Particle/Dust/Dust.h"
int Dust::countId;

/// <summary>
/// コンストラクタ
/// </summary>
Dust::Dust()
{
	// ID
	id_ = 0;

	// 放出されているかどうか（放出フラグ）
	isEmission_ = false;

	// 図形
	shape_.scale = { 2.0f , 2.0f };
	shape_.theta = 0.0f;
	shape_.translate = { 0.0f , 0.0f };

	// 位置
	pos_.local = { {-1.0f , 1.0f} , {1.0f , 1.0f} , {-1.0f , -1.0f} , {1.0f , -1.0f} };
	LocalToScreen();

	// 移動速度
	vel_ = { 0.0f , 0.0f };

	// ベクトル
	vec_ = { 0.0f , 0.0f };

	// フレーム
	frame_.current = 0;
	frame_.end = 0;
	frame_.rate = 0.0f;

	// 不透明度
	alpha_ = 0;
}

/// <summary>
/// 放出する
/// </summary>
/// <param name="posX">X軸の位置</param>
/// <param name="posY">Y軸の位置</param>
/// <param name="theta">角度</param>
void Dust::Emission(float posX, float posY, float theta)
{
	if (isEmission_ == false)
	{
		// 放出する（放出フラグをtrueにする）
		isEmission_ = true;

		// ID
		id_ = ++countId;

		// 位置
		shape_.translate = { posX , posY };
		shape_.theta = theta;
		LocalToScreen();

		// ベクトル
		vec_ = { cosf((theta / 180.0f) * float(M_PI)) , sinf((theta / 180.0f) * float(M_PI)) };

		// 移動速度
		vel_ = { 1.0f , 1.0f };

		// フレーム
		frame_.current = 0;
		frame_.end = 180;
		frame_.current = 0;

		// 不透明度
		alpha_ = 0;
	}
}

/// <summary>
/// 動かす
/// </summary>
void Dust::Move()
{
	if (isEmission_)
	{
		// 図形の操作

		shape_.translate.x += vel_.x * vec_.x;
		shape_.translate.y += vel_.y * vec_.y;

		shape_.theta += 1.0f;

		if (shape_.theta >= 360.0f)
		{
			shape_.theta = 0.0f;
		}

		LocalToScreen();


		// フレーム操作

		frame_.current++;

		// 0 ~ 90 透明度を下げる
		if (frame_.current < 90)
		{
			frame_.rate = static_cast<float>(frame_.current) / 90.0f;

			alpha_ = static_cast<int>((1.0f - frame_.rate) * 0.0f + frame_.rate * 16.0f);
		}
		else if (frame_.current >= 90)
		{
			// 90 ~ 180 透明度を上げる

			frame_.rate = static_cast<float>(frame_.current - 90) / 90.0f;

			alpha_ = static_cast<int>((1.0f - frame_.rate) * 16.0f + frame_.rate * 0.0f);
		}

		// フレームが終了したら消える（放出フラグがfalseになる）
		if (frame_.current > frame_.end)
		{
			isEmission_ = false;

			// IDを消す
			id_ = 0;
			--countId;
		}
	}
}

/// <summary>
/// 描画する
/// </summary>
/// <param name="gh">画像</param>
void Dust::Draw(int gh)
{
	if (isEmission_)
	{
		Novice::DrawQuad
		(
			static_cast<int>(pos_.screen.leftTop.x), static_cast<int>(pos_.screen.leftTop.y),
			static_cast<int>(pos_.screen.rightTop.x), static_cast<int>(pos_.screen.rightTop.y),
			static_cast<int>(pos_.screen.leftBottom.x), static_cast<int>(pos_.screen.leftBottom.y),
			static_cast<int>(pos_.screen.rightBottom.x), static_cast<int>(pos_.screen.rightBottom.y),
			0, 0, 1, 1, gh, 0xFFFF0000 + alpha_
		);
	}
}