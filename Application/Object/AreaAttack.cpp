#include "AreaAttack.h"

#include "SphereCollider/SphereCollider.h"
#include "ImGuiManager/ImGuiManager.h"

void AreaAttack::Initialize(const Infomation& info)
{
	InstancingGameObject::Initialize("sphere");

	collider_.reset(new SphereCollider);
	collider_->Initialize("area", colliderWorld_);

	data_ = info;
	data_.power_ = 1.0f;
	aliveFrame_ = 20;

	world_.Initialize();
	colliderWorld_.Initialize();
	world_.translate_ = data_.popPosition_;
	colliderWorld_.parent_ = &world_;

	colliderWorld_.translate_.y = 2.0f;
	
	collider_->SetRadius(data_.power_);

	isActive_ = true;
	//collider_->SetColor({ 0.0f,0.0f,0.0f,1.0f });
}

void AreaAttack::Update()
{
	if (!isActive_)
	{
		return;
	}

	aliveFrame_--;
	if (aliveFrame_ <= 0)
	{
		color = { 0.0f,0.0f,0.0f,0.0f };
		isActive_ = false;
	}

	//world_.translate_ += direct3_ * (data_.power_ * 0.1f);

	//行列更新
	world_.UpdateMatrix();
	colliderWorld_.UpdateMatrix();
	collider_->Update();
	//sCollider_->Update();
	//タグに対応したモデルにワールド追加
	IMM_->SetData(tag_, colliderWorld_);
}

void AreaAttack::Draw()
{
	if (!isActive_)
	{
		return;
	}
	collider_->Draw();
}

void AreaAttack::DebugWindow()
{
	collider_->Debug("area");

	ImGui::Begin(tag_.c_str());

	ImGui::Text("tra : %.2f,%.2f", world_.translate_.x, world_.translate_.z);
	ImGui::Separator();

	ImGui::End();
}

void AreaAttack::OnCollision()
{
}
