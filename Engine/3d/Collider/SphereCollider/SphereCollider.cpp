#include "SphereCollider.h"
#include"InstancingModelManager/InstancingModelManager.h"
#include"ColliderOBB/OBBCollider.h"
#include"IsCollisions.h"
#include<imgui.h>


bool SphereCollider::isDraw_ = true;

SphereCollider::SphereCollider() {
	world_.scale_ = { radius_,radius_,radius_ };
}


void SphereCollider::Initialize(const std::string& tag, const WorldTransform&world) {
	InstancingGameObject::Initialize("sphere");

	world_.parent_=(&world);

	colliderTag_ = tag;
	
	IMM_->SetFillMode(tag_, FillMode::kWireFrame);
	IMM_->SetAlpha(tag_, alpha_);
	IMM_->SetEnableTexture(tag_,false);
	IMM_->SetEnableShader(tag_, false);
}


void SphereCollider::Update() {
	preWorld_ = world_;

	world_.UpdateMatrix();
}

void SphereCollider::Draw() {
#ifdef _DEBUG
	if (isDraw_) {
		InstancingModelManager::GetInstance()->SetWorld(tag_, world_);
	}
#endif // _DEBUG

}

bool SphereCollider::IsCollision(const SphereCollider& sphere, Vector3& backVec) {
	//各点取得
	Vector3 pos = sphere.world_.GetMatWorldTranslate();
	Vector3 myP = world_.GetMatWorldTranslate();

	//各半径取得
	float wide = sphere.radius_;
	float myW = radius_;

	//距離計算
	Vector3 sub = myP - pos;

	//ヒットチェック
	if (Length(sub) < wide + myW) {

		//正規化
		sub.SetNormalize();

		//基コライダーから自分への向きベクトル作成
		backVec = sub * (wide, myW);

		//返却
		return true;
	}

	//当たっていない
	return false;
}



bool SphereCollider::IsCollision(const OBBCollider& obb, Vector3& backVec)
{

	Vector3 rotate = GetAllRotate(obb.GetWorld());

#pragma region OBBのワールド行列をスケールなしで作成
	//回転行列
	Matrix4x4 rotateM = MakeRotateXMatrix(rotate.x) * (MakeRotateYMatrix(rotate.y) * MakeRotateZMatrix(rotate.z));
	//座標行列
	Matrix4x4 translateM = MakeTranslateMatrix(obb.GetWorld().translate_);
	//スケールは使わない（sizeで使う
	Matrix4x4 scaleM = MakeIdentity4x4();
	//OBBのworld行列生成
	Matrix4x4 OBBM = scaleM * (rotateM * translateM);

	//逆行列
	Matrix4x4 inverseM = Inverse(OBBM);
#pragma endregion

	
	//スフィアコライダーの座標をOBBのローカル空間に出る
	Vector3 sphereLocal = Transform(world_.GetMatWorldTranslate(), inverseM);
	//
	Vector3 size = { obb.GetWorld().scale_.x,obb.GetWorld().scale_.y,obb.GetWorld().scale_.z};

	//AABB取得
	AABB aabb_ = { .minV = -size,.maxV = size };
	//Sphere取得
	Sphere sphere = { sphereLocal,radius_ };

	//当たり判定
	Vector3 saikin{};
	if (InCollision(aabb_, sphere, saikin)) {
		//色の変更
		IMM_->SetColor(tag_, hitColor);

		//押し出しベクトルの計算

		//それをOBBローカル空間からワールド空間へ

		return true;
	}
	else {
		//色の変更
		IMM_->SetColor(tag_, normalColor);
		return false;
	}
}

void SphereCollider::Debug(const char* name) {
#ifdef _DEBUG
	std::string cName = name;
	cName = cName + " collider";

	if (ImGui::BeginMenu(cName.c_str())) {
		ImGui::DragFloat("alpha", &alpha_, 0.01f);
		ImGui::DragFloat("radius", &radius_, 0.1f);
		ImGui::Checkbox("isDrawAll", &isDraw_);
		ImGui::EndMenu();
	}

	IMM_->SetAlpha(tag_, alpha_);
	world_.scale_ = { radius_,radius_,radius_ };

#endif // _DEBUG

	
	

}



