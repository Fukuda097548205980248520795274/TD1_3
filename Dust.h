#pragma once
#include "./Class/Object/Particle/Particle.h"

class Dust : public Particle
{
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Dust();

	/// <summary>
	/// ���o����
	/// </summary>
	/// <param name="posX">X���̈ʒu</param>
	/// <param name="posY">Y���̈ʒu</param>
	/// <param name="theta">�p�x</param>
	void Emission(float posX, float posY, float theta);

	/// <summary>
	/// ������
	/// </summary>
	void Move();

	/// <summary>
	/// �`�悷��
	/// </summary>
	/// <param name="gh">�摜</param>
	void Draw(int gh) ;

	// ID�ɓ����l
	static int countId;

	// �����x
	int alpha_;
};

