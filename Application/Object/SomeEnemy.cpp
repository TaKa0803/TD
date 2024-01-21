#include "SomeEnemy.h"

#include "ImGuiManager/ImGuiManager.h"

void SomeEnemy::Initialize()
{
	InstancingGameObject::Initialize("player");
	world_.scale_ = { 1.0f,1.0f,1.0f };
	world_.UpdateMatrix();

	collider_.reset(new SphereCollider);
	collider_->Initialize("enemy", world_);

}

void SomeEnemy::Update()
{
	world_.UpdateMatrix();
	collider_->Update();
}

void SomeEnemy::DebagWindow()
{
	collider_->Debug("enemy");

	ImGui::Begin("SomeEnemy");



	ImGui::End();
}

void SomeEnemy::Draw()
{
	collider_->Draw();
	IMM_->SetWorld(tag_, world_);
}

