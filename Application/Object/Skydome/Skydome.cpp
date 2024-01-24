#include "Skydome.h"

Skydome::Skydome()
{
	InstancingGameObject::Initialize("SkyDome");
}

void Skydome::Initialize()
{

	world_.scale_ = { 500,500,500 };

}



void Skydome::Draw()
{
InstancingGameObject::Update();
}
