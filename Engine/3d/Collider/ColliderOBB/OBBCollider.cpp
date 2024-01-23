#include "OBBCollider.h"
#include<imgui.h>
#include"SphereCollider/SphereCollider.h"

#include"IsCollisions.h"

bool OBBCollider::isDraw_ = true;

void OBBCollider::Initialize(const std::string& tag, const WorldTransform& parent)
{
	//院スタン寝具の初期化
	InstancingGameObject::Initialize("box");
	//親子関係設定
	world_.parent_=(&parent);
	//コライダーのタグ設定
	colliderTag_ = tag;

	//画像を切る
	IMM_->SetEnableTexture(tag_, false);
	//ワイヤーフレーム表示
	IMM_->SetFillMode(tag_, FillMode::kWireFrame);
	//影の削除
	IMM_->SetEnableShader(tag_, false);
	//透明度設定
	IMM_->SetAlpha(tag_, alpha_);
}

void OBBCollider::Update()
{
	preWorld_ = world_;
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

	ImGui::ColorEdit4("hit color", &hitColor.x);
	ImGui::Checkbox("isDraw", &isDraw_);
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

	//AABB取得
	AABB aabb_ = { .minV = -size,.maxV = size };
	//Sphere取得
	Sphere sphere={ sphereLocal,collider->GetRadius() };

	//当たり判定
	Vector3 saikin{};
	if (InCollision(aabb_, sphere,saikin)) {
		//色の変更
		IMM_->SetColor(tag_,hitColor);

		//押し出しベクトルの計算


		return true;
	}
	else {
		//色の変更
		IMM_->SetColor(tag_, normalColor);
		return false;
	}
	 
}
