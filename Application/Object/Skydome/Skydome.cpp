#include "Skydome.h"

Skydome::Skydome()
{
	InstancingGameObject::Initialize("SkyDome");
	IMM_->SetEnableShader(tag_, false);
}

void Skydome::Initialize()
{
	world_.SetScale(100.0f);

}



void Skydome::Draw()
{
InstancingGameObject::Update();
}
