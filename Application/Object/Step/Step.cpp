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
	
}

void Step::Draw()
{
	InstancingGameObject::Update();
}

void Step::Debug()
{
	world_.DrawDebug("step");
}
