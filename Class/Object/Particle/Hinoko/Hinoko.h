#pragma once
#include "./Class/Object/Particle/Particle.h"

class Hinoko : public Particle
{
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Hinoko();

	/// <summary>
	/// ���o����
	/// </summary>
	void Emission(float posX, float posY, float theta, int time);

	/// <summary>
	/// ������
	/// </summary>
	void Move();

	/// <summary>
	/// �`�悷��
	/// </summary>
	/// <param name="gh">�摜</param>
	void Draw(int gh) override;

	// ID�𐔂���l
	static int countId;

	// �N�[���^�C��
	static int coolTime;

	// �s�����x
	int alpha_;
};