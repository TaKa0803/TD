#include "OBBCollider.h"
#include<imgui.h>
#include"SphereCollider/SphereCollider.h"
#include <algorithm>

//AABBと円
bool InCollision(const AABB& a, const Sphere& s) {
	//最近接点を求める
	Vector3 closestPoint{ std::clamp(s.center.x,a.minV.x,a.maxV.x),
	std::clamp(s.center.y,a.minV.y,a.maxV.y),
	std::clamp(s.center.z,a.minV.z,a.maxV.z)
	};
	Sphere S{
		.center = closestPoint,
		.radius = 0.01f,
	};


	Vector3 V = closestPoint- s.center;

	//最近接点と球の中心との距離を求める
	float dis = Length(V);
	//距離が半径よりも小さければ衝突
	if (dis <= s.radius) { return true; }
	return false;
}

void OBBCollider::Initialize(const std::string& tag, const WorldTransform& parent)
{
	InstancingGameObject::Initialize("box");

	world_.SetParent(&parent);

	colliderTag_ = tag;

	IMM_->SetEnableTexture(tag_, false);
	IMM_->SetFillMode(tag_, FillMode::kWireFrame);
}

void OBBCollider::Update()
{

	/*Matrix4x4 rotateXM = MakeRotateAxisAngle({ 1,0,0 }, rotation_.x);
	Matrix4x4 rotateYM = MakeRotateAxisAngle({ 0,1,0 }, rotation_.y);
	Matrix4x4 rotateZM = MakeRotateAxisAngle({ 0,0,1 }, rotation_.z);

	Matrix4x4 rotateM = rotateXM * (rotateYM * rotateZM);

	Matrix4x4 scaleM = MakeScaleMatrix(world_.scale_);

	Matrix4x4 tlansM = MakeTranslateMatrix(world_.translate_);

	Matrix4x4 worldM = scaleM * (rotateM * tlansM);

	if (&world_.GetParent() != nullptr) {
		Matrix4x4 pare = world_.parent_->matWorld_;
		world_.matWorld_ =  worldM * pare;
	}
	else {
		world_.matWorld_ = worldM;
	}*/

	world_.UpdateMatrix();
}

void OBBCollider::Draw()
{
#ifdef _DEBUG
	if (isDraw_) {
		InstancingModelManager::GetInstance()->SetWorld(tag_, world_);
	}
#endif // _DEBUG
}

void OBBCollider::Debug(const char* name)
{
	ImGui::Begin(name);
	ImGui::DragFloat3("trans", &world_.translate_.x, 0.10f);
	ImGui::DragFloat3("rotation", &world_.rotate_.x, 0.01f);
	ImGui::DragFloat3("scale", &world_.scale_.x, 0.01f);

	ImGui::Text("min %4.1f , %4.1f , %4.1f",aabb_.minV.x, aabb_.minV.y, aabb_.minV.z);
	ImGui::Text("max %4.1f , %4.1f , %4.1f", aabb_.maxV.x, aabb_.maxV.y, aabb_.maxV.z);

	ImGui::End();

}

bool OBBCollider::IsCollision(SphereCollider* collider)
{
	//回転行列
	Matrix4x4 rotateM = MakeRotateXMatrix(world_.rotate_.x) * (MakeRotateYMatrix(world_.rotate_.y)*MakeRotateZMatrix(world_.rotate_.z));
	//座標行列
	Matrix4x4 translateM = MakeTranslateMatrix(world_.translate_);
	//スケールは使わない（sizeで使う
	Matrix4x4 scaleM = MakeIdentity4x4();
	//OBBのworld行列生成
	Matrix4x4 OBBM = scaleM * (rotateM * translateM);
	
	//逆行列
	Matrix4x4 inverseM = Inverse(OBBM);

	//スフィアコライダーの座標をOBBのローカル空間に出る
	Vector3 sphereLocal = Transform(collider->GetWorld().GetMatWorldTranslate(), inverseM);
	//
	Vector3 size = { world_.scale_.x,world_.scale_.y,world_.scale_.z };

	aabb_ = { .minV = -size,.maxV = size };
	Sphere sphere={ sphereLocal,collider->GetScale() };

	if (InCollision(aabb_, sphere)) {

		IMM_->SetColor(tag_,hitColor);

		return true;
	}
	else {
		IMM_->SetColor(tag_, normalColor);


		return false;
	}
	 
}
