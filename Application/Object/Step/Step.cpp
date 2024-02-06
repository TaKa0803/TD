#include"Step.h"

Step::Step()
{
	InstancingGameObject::Initialize("step");
	world_.scale_ = { 9.1f,2,3 };

	for (int i = 0; i < 3; i++) {
		for (int h = 0; h < 5; h++) {
			audience_[i][h] = std::make_unique<Audience>();
		}
	}
}

void Step::Initialize()
{
	for (int i = 0; i < 3; i++) {
		for (int h = 0; h < 5; h++) {
			audience_[i][h]->Initialize();
			audience_[i][h]->SetParent(&world_);
		}
	}
}

void Step::Update()
{
	InstancingGameObject::Update();
	for (int i = 0; i < 3; i++) {
		for (int h = 0; h < 5; h++) {
			audience_[i][h]->SetParent(&world_);
		}
	}

	for (int i = 0; i < 3; i++) {
		for (int h = 0; h < 5; h++) {
			audience_[i][h]->Update();
		}
	}
	
}

void Step::Draw()
{

	for (int i = 0; i < 3; i++) {
		for (int h = 0; h < 5; h++) {
//			audience_[i][h]->Draw();
		}
	}
	InstancingGameObject::Draw();
}

void Step::Debug()
{
	for (int i = 0; i < 3; i++) {
		for (int h = 0; h < 5; h++) {
			audience_[i][h]->Debug("audience");
		}
	}
	world_.DrawDebug("step");
}
