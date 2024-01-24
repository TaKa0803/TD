#include "Skydome.h"

Skydome::Skydome()
{
	InstancingGameObject::Initialize("SkyDome");
}

void Skydome::Initialize()
{
	world_.SetScale(500.0f);

}



void Skydome::Draw()
{
InstancingGameObject::Update();
}
