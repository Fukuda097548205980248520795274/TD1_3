#include"./Class/Object/Particle/Particle.h"

Particle::Particle()
{
    // frame_ をここで初期化
    frame_.current = 0;
    frame_.end = 0;
    frame_.rate = 0.0f;

    isEmission_ = false;

    vec_ = { 0.0f,0.0f };
}

void Particle::Move()
{

}

