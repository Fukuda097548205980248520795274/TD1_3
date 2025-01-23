#include"./Class/Object/Particle/Snow/Snow.h"

Snow::Snow()
{
	isEmission_ = false;

	// shape_の初期化
	shape_.scale = { 1.0f, 1.0f };
	shape_.theta = 0.0f;
	shape_.translate = { 0.0f, 0.0f };

	// 加速度
	acceleration_ = { 0.0f, 0.0f };

	// 質量
	mass_ = 6.0f;

	// 位置の初期化
	pos_.local = { {-1.0f, 1.0f}, {1.0f, 1.0f}, {-1.0f, -1.0f}, {1.0f, -1.0f} };
	LocalToScreen();

	// 移動速度
	vel_ = { 0.0f, 0.0f };

	// ベクトル
	vec_ = { 0.0f, 0.0f };

	// フレームの初期化
	frame_.current = 0;
	frame_.end = 0;
	frame_.rate = 0.0f;

	// 不透明度
	alpha_ = 0;

	SnoWGravity_ = -2.0f;

}

void Snow::Emission()
{

	if (isEmission_)
	{
		return;
	}

	// 放出する（放出フラグがtrueになる）
	isEmission_ = true;

	//図形
	shape_.scale = { 2.0f,2.0f };
	shape_.theta = 0.0f;
	shape_.translate = { static_cast <float>(rand() % 1280), 900.0f };
	// 加速度
	acceleration_ = { 0.0f , 0.0f };

	// 移動速度
	vel_ = { 0.0f , 0.0f };

	// 不透明度
	alpha_ = 0;
}

void Snow::Move()
{
	if (!isEmission_)
	{
		return;
	}

	frame_.current++;


	if (isEmission_)
	{

		// 空気抵抗
		float airResistanceX = kAir * -vel_.x;
		float airResistanceY = kAir * -vel_.y;

		// 空気抵抗の加速度
		float airResistanceAccelerationX = airResistanceX / mass_;
		float airResistanceAccelerationY = airResistanceY / mass_;



		// 加速度
		acceleration_.y = SnoWGravity_ + airResistanceAccelerationY;

		// 横方向のランダムな風の影響を追加
		float wind = ((rand() % 200) - 100) / 10000.0f; // -0.01f ～ 0.01f の範囲のランダム値
		acceleration_.x = airResistanceAccelerationX + wind;

		// 加速度を加算する
		vel_.x += acceleration_.x / 60.0f; // 横方向
		vel_.y += acceleration_.y / 60.0f;

		// 動かす
		shape_.translate.x += vel_.x;
		shape_.translate.y += vel_.y;
		LocalToScreen();

		if (pos_.screen.rightTop.y > 720.0f)
		{
			isEmission_ = false;
		}

	}
}

void Snow::Draw()
{
	if (!isEmission_)
	{
		return;
	}

	///画像入れる
	int ghWhite = Novice::LoadTexture("./NoviceResources/white1x1.png");



	Novice::DrawQuad
	(
		static_cast<int>(pos_.screen.leftTop.x), static_cast<int>(pos_.screen.leftTop.y),
		static_cast<int>(pos_.screen.rightTop.x), static_cast<int>(pos_.screen.rightTop.y),
		static_cast<int>(pos_.screen.leftBottom.x), static_cast<int>(pos_.screen.leftBottom.y),
		static_cast<int>(pos_.screen.rightBottom.x), static_cast<int>(pos_.screen.rightBottom.y),
		0, 0, 48, 32, ghWhite, 0xFFFFFFFF
	);
}