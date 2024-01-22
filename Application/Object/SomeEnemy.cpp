#include "SomeEnemy.h"

#include "ImGuiManager/ImGuiManager.h"

void SomeEnemy::Initialize()
{
	InstancingGameObject::Initialize("sphere");
	world_.Initialize();
	world_.scale_ = { 1.0f,1.0f,1.0f };
	world_.translate_.y = 2.0f;
	world_.UpdateMatrix();

	collider_.reset(new SphereCollider);
	collider_->Initialize("enemy", world_);

}

void SomeEnemy::Update()
{
	world_.UpdateMatrix();
	collider_->Update();
}

void SomeEnemy::DebagWindow(int num)
{
	collider_->Debug("enemy");

	ImGui::Begin("SomeEnemy");

	ImGui::DragFloat3(std::to_string(num).c_str(), &world_.translate_.x, 0.02f);

	ImGui::End();
}

void SomeEnemy::Draw()
{
	collider_->Draw();
	IMM_->SetWorld(tag_, world_);
}

