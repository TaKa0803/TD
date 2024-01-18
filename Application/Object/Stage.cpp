#include "Stage.h"
#include <imgui.h>

Stage::Stage()
{
	GameObject::Initialize("Stage");
}

void Stage::Initialize()
{
	world_.Initialize();
	world_.translate_.y = 1.0f;
	world_.scale_.x = kMaxRadius_ * kTransformRadius_;
	world_.scale_.z = kMaxRadius_ * kTransformRadius_;
	//mode_ = None;
	world_.UpdateMatrix();


}

void Stage::Update()
{
	world_.UpdateMatrix();
}

void Stage::DebagWindow()
{
	model_->DebugParameter("Stage");

	ImGui::Begin("Stage");

	ImGui::DragFloat("maxRadius", &kMaxRadius_, 0.1f);

	ImGui::DragFloat("scale", &kTransformRadius_, 0.01f);

	ImGui::End();

	world_.scale_.x = kMaxRadius_ * kTransformRadius_;
	world_.scale_.z = kMaxRadius_ * kTransformRadius_;

}

void Stage::Draw(const Matrix4x4& viewp)
{
	GameObject::Draw(viewp);
}
