#include "Plane.h"
#include<imgui.h>

Plane::Plane() {
	GameObject::Initialize("stage");
}

void Plane::Initialize() {
	
	world_.scale_ = { 11,3,11 };
	
	world_.UpdateMatrix();
}

void Plane::DebagWindow() {
	
#ifdef _DEBUG
	float scale = world_.scale_.x;

	ImGui::Begin("stage");
	model_->DebugParameter("stage");
	ImGui::DragFloat3("scale", &world_.scale_.x);
	ImGui::End();

	world_.UpdateMatrix();
#endif // _DEBUG
	
}

void Plane::Draw(const Matrix4x4& viewp) {
	GameObject::Draw(viewp);
}
