#include"./Class/Object/Particle/Snow/Snow.h"

Snow::Snow()
{
	isEmission_ = false;

	// shape_�̏�����
	shape_.scale = { 1.0f, 1.0f };
	shape_.theta = 0.0f;
	shape_.translate = { 0.0f, 0.0f };

	// �����x
	acceleration_ = { 0.0f, 0.0f };

	// ����
	mass_ = 6.0f;

	// �ʒu�̏�����
	pos_.local = { {-1.0f, 1.0f}, {1.0f, 1.0f}, {-1.0f, -1.0f}, {1.0f, -1.0f} };
	LocalToScreen();

	// �ړ����x
	vel_ = { 0.0f, 0.0f };

	// �x�N�g��
	vec_ = { 0.0f, 0.0f };

	// �t���[���̏�����
	frame_.current = 0;
	frame_.end = 0;
	frame_.rate = 0.0f;

	// �s�����x
	alpha_ = 0;

	SnoWGravity_ = -2.0f;

}

void Snow::Emission()
{

	if (isEmission_)
	{
		return;
	}

	// ���o����i���o�t���O��true�ɂȂ�j
	isEmission_ = true;

	//�}�`
	shape_.scale = { 2.0f,2.0f };
	shape_.theta = 0.0f;
	shape_.translate = { static_cast <float>(rand() % 1280), 900.0f };
	// �����x
	acceleration_ = { 0.0f , 0.0f };

	// �ړ����x
	vel_ = { 0.0f , 0.0f };

	// �s�����x
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

		// ��C��R
		float airResistanceX = kAir * -vel_.x;
		float airResistanceY = kAir * -vel_.y;

		// ��C��R�̉����x
		float airResistanceAccelerationX = airResistanceX / mass_;
		float airResistanceAccelerationY = airResistanceY / mass_;



		// �����x
		acceleration_.y = SnoWGravity_ + airResistanceAccelerationY;

		// �������̃����_���ȕ��̉e����ǉ�
		float wind = ((rand() % 200) - 100) / 10000.0f; // -0.01f �` 0.01f �͈̔͂̃����_���l
		acceleration_.x = airResistanceAccelerationX + wind;

		// �����x�����Z����
		vel_.x += acceleration_.x / 60.0f; // ������
		vel_.y += acceleration_.y / 60.0f;

		// ������
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

	///�摜�����
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