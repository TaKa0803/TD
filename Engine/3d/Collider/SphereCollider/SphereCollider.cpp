#include "SphereCollider.h"
#include"InstancingModelManager/InstancingModelManager.h"
#include"ColliderOBB/OBBCollider.h"
#include"IsCollisions.h"
#include<imgui.h>


bool SphereCollider::isDraw_ = true;

SphereCollider::SphereCollider() {

}


void SphereCollider::Initialize(const std::string& tag, const WorldTransform& world) {
	InstancingGameObject::Initialize("sphere");

	world_.parent_ = (&world);

	colliderTag_ = tag;

	world_.scale_ = { radius_,radius_,radius_ };

	IMM_->SetFillMode(tag_, FillMode::kWireFrame);
	IMM_->SetAlpha(tag_, alpha_);
	IMM_->SetEnableTexture(tag_, false);
	IMM_->SetEnableShader(tag_, false);
}

void SphereCollider::Initialize(const std::string& tag)
{
	InstancingGameObject::Initialize("sphere");

	colliderTag_ = tag;

	world_.scale_ = { radius_,radius_,radius_ };

	IMM_->SetFillMode(tag_, FillMode::kWireFrame);
	IMM_->SetAlpha(tag_, alpha_);
	IMM_->SetEnableTexture(tag_, false);
	IMM_->SetEnableShader(tag_, false);
}


void SphereCollider::Update() {
	preWorld_ = world_;

	world_.UpdateMatrix();
}

void SphereCollider::Draw() {
#ifdef _DEBUG
	if (isDraw_) {
		InstancingModelManager::GetInstance()->SetData(tag_, world_, color);
	}
#endif // _DEBUG

}

bool SphereCollider::IsCollision(const SphereCollider& sphere, Vector3& backVec) {
	//各点取得
	Vector3 pos = sphere.world_.GetMatWorldTranslate();
	Vector3 myP = world_.GetMatWorldTranslate();

	//各半径取得
	float wide = GetAllScaleX(sphere.GetWorld());
	float myW = GetAllScaleX(world_);

	//距離計算
	Vector3 sub = myP - pos;

	Vector3 leng = sub;
	//ヒットチェック
	if (Length(sub) < wide + myW) {

		//正規化
		sub.SetNormalize();

		//基コライダーから自分への向きベクトル作成
		backVec = sub * (wide, myW);

		//押し出し量求める
		backVec = sub - leng;

		//返却
		return true;
	}

	//当たっていない
	return false;
}



bool SphereCollider::IsCollision(OBBCollider& obb, Vector3& backVec)
{


#pragma region OBBのワールド行列をスケールなしで作成

	//OBBのworld行列生成
	Matrix4x4 OBBM = obb.GetWorldM();

	//逆行列
	Matrix4x4 inverseM = obb.GetInverseWorldM();
#pragma endregion


	//スフィアコライダーの座標をOBBのローカル空間に出る
	Vector3 sphereLocal = Transform(world_.GetMatWorldTranslate(), inverseM);
	//すべてのスケールからサイズ取得
	Vector3 size = GetAllScale(obb.GetWorld());

	//AABB取得
	AABB aabb_ = { .minV = -size,.maxV = size };
	//Sphere取得
	Sphere sphere = { sphereLocal,radius_ };

	//当たり判定
	Vector3 saikin{};
	if (InCollision(aabb_, sphere, saikin)) {


		//OBBLocalPosCange
		saikin = Transform(saikin, OBBM);

		//mosionajiiti
		if (world_.GetMatWorldTranslate() == saikin) {
			//スフィアコライダーの座標をOBBのローカル空間に出る
			sphereLocal = Transform(preWorld_.GetMatWorldTranslate(), inverseM);
			//Sphere取得
			sphere = { sphereLocal,radius_ };
			InCollision(aabb_, sphere, saikin);

			saikin = Transform(saikin, OBBM);

			Vector3 velo = preWorld_.GetMatWorldTranslate() - saikin;
			velo.SetNormalize();
			velo *= radius_;

			backVec = velo;

		}
		else {
			///押し出しベクトルを利用して計算
			//最近接点から円の中心点への向きベクトルを算出
			Vector3 velo = world_.GetMatWorldTranslate() - saikin;
			//正規化

			Vector3 norVe = velo;
			norVe.SetNormalize();
			//半径分伸ばす
			norVe *= radius_;

			//渡す
			backVec = norVe - velo;


		}

#ifdef _DEBUG
		////最近接点描画
		WorldTransform sWo;
		sWo.translate_ = saikin;
		sWo.scale_ = { 0.1f,0.1f,0.1f };
		sWo.UpdateMatrix();
		IMM_->SetData("sphere", sWo);
#endif // _DEBUG
		obb.SetColor(true);
		//色の変更
		SetColor(true);
		return true;
	}
	else {

#ifdef _DEBUG
		//最近接点描画
		WorldTransform sWo;
		sWo.translate_ = Transform(saikin, OBBM);
		sWo.scale_ = { 0.1f,0.1f,0.1f };
		sWo.UpdateMatrix();
		IMM_->SetData("sphere", sWo);
#endif // _DEBUG

		//色の変更
		SetColor(false);
		obb.SetColor(false);

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

void SphereCollider::SetColor(bool hit)
{
	if (hit) {
		color = hitColor;
	}
	else {
		color = normalColor;
	}
}

void SphereCollider::UpdateMatrix()
{
	world_.UpdateMatrix();
}




