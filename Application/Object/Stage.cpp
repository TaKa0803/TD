#include "Stage.h"
#include <imgui.h>

Stage::Stage()
{
	GameObject::Initialize("player");
}

void Stage::Initialize()
{
	world_.Initialize();
	world_.translate_.y = 1.0f;
	world_.scale_.x = radius_ * kTransformRadius_;
	world_.scale_.z = radius_ * kTransformRadius_;
	mode_ = None;
	world_.UpdateMatrix();
}

void Stage::Update()
{
	switch (mode_)
	{
	case Stage::None:
		break;
	case Stage::Shrink:
		if (5.0f < radius_)
		{
			radius_ -= 0.03f;
		}
		else
		{
			radius_ = kMaxRadius_ * 0.5f;
		}
		break;
	case Stage::Expand:
		radius_ += 2.5f;
		if (kMaxRadius_ < radius_)
		{
			radius_ = kMaxRadius_;
		}
		mode_ = Shrink;
		break;
	default:
		break;
	}
	world_.scale_.x = radius_ * kTransformRadius_;
	world_.scale_.z = radius_ * kTransformRadius_;
	world_.UpdateMatrix();
}

void Stage::DebagWindow()
{
	model_->DebugParameter("player");
#ifdef _DEBUG

	ImGui::Begin("Stage");

	if (ImGui::RadioButton("None", mode_ == None))
		mode_ = None;
	if (ImGui::RadioButton("Shrink", mode_ == Shrink))
		mode_ = Shrink;
	if (ImGui::RadioButton("Expand", mode_ == Expand))
		mode_ = Expand;

	if (ImGui::GetIO().KeysDown[ImGuiKey_Space])
	{
		mode_ = Expand;
	}

	ImGui::End();

#endif // _DEBUG

}

void Stage::Draw(const Matrix4x4& viewp)
{
	GameObject::Draw(viewp);
}
