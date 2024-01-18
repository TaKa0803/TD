#include "SphereCollider.h"
#include"InstancingModelManager/InstancingModelManager.h"
#include"ColliderPlane/PlaneCollider.h"
#include<imgui.h>

SphereCollider::SphereCollider() {
	world_.scale_ = { wide_,wide_,wide_ };
}


void SphereCollider::Initialize(const std::string& tag, const WorldTransform&world) {
	InstancingGameObject::Initialize("sphere");

	world_.SetParent(&world);

	colliderTag_ = tag;
	
	//IMM_->SetFillMode(tag_, FillMode::kWireFrame);
	//IMM_->SetAlpha(tag_, alpha_);
	//IMM_->SetEnableTexture(tag_,false);
}


void SphereCollider::Update() {
	preWorld_ = world_;

	world_.scale_ = { wide_,wide_,wide_ };
	world_.UpdateMatrix();
}

void SphereCollider::Draw() {
#ifdef _DEBUG
	if (isDraw_) {
		InstancingModelManager::GetInstance()->SetWorld(tag_, world_);
	}
#endif // _DEBUG

}

bool SphereCollider::IsHit(const SphereCollider& sphere, Vector3& backVec) {
	Vector3 pos = sphere.world_.GetMatWorldTranslate();
	Vector3 myP = world_.GetMatWorldTranslate();

	float wide = sphere.wide_;
	float myW = wide_;

	Vector3 sub = myP - pos;

	//はんいない
	if (Length(sub) < wide + myW) {

		sub.SetNormalize();

		//基コライダーから自分への向きベクトル
		backVec = sub * (wide, myW);

		return true;
	}


	return false;
}

bool SphereCollider::IsHit(const PlaneCollider& plane, Vector3& beckVec)
{
	

	return false;
}

void SphereCollider::Debug(const char* name) {

	


#ifdef _DEBUG
	float alpha = IMM_->GetAlpha(tag_);

	std::string cName = name;
	cName = cName + " collider";

	if (ImGui::BeginMenu(cName.c_str())) {
		ImGui::DragFloat("alpha", &alpha, 0.01f);
		ImGui::DragFloat("wide", &wide_, 0.1f);
		ImGui::Checkbox("isDraw", &isDraw_);
		ImGui::EndMenu();
	}

	IMM_->SetAlpha(tag_, alpha);
	world_.scale_ = { wide_,wide_,wide_ };

#endif // _DEBUG

	
	

}

void SphereCollider::SetAlpha(const float alpha)
{
	IMM_->SetAlpha(tag_, alpha);
}

