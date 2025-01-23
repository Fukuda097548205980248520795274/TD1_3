#pragma once
#include"./Class/Object/Particle/Particle.h"

class Snow : public Particle
{
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Snow();


	void Emission();

	void Move();

	void Draw();

	int isEmission_;

	struct Vec2 acceleration_;

	float mass_;

	// ID�ɓ����l
	static int countId;

	// �����x
	int alpha_;


	// �x�N�g��
	struct Vec2 vec_;

	// �t���[��
	struct Frame frame_;

	float SnoWGravity_;
};

