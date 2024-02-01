#include"Step.h"

Step::Step()
{
	InstancingGameObject::Initialize("step");
	world_.scale_ = { 9.1f,2,3 };
}

void Step::Initialize()
{
	
}

void Step::Update()
{
	InstancingGameObject::Update();
}

void Step::Draw()
{
	InstancingGameObject::Draw();
}

void Step::Debug()
{
	world_.DrawDebug("step");
}
