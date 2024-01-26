#include "BossEnemy.h"

#include "ImGuiManager/ImGuiManager.h"
#include "SomeEnemy.h"

BossEnemy::BossEnemy()
{
	GameObject::Initialize("ALPlayer");

#pragma region HPゲージ関連
	//HPバー
	int tex = TextureManager::LoadTex(hpTex_);
	Vector2 pos = {
		958.4f,
		60
	};
	hpBar_.reset(Sprite::Create(tex, { 962,58 }, { 962,58 }, { 962,58 }, pos, { 1.0f,0.5f }));

	//HPバー背景
	tex = TextureManager::LoadTex(hpBackTex_);
	hpBarBack_.reset(Sprite::Create(tex, { 962,58 }, { 962,58 }, { 962,58 }, { 640,60 }));

	//フレーム画像
	tex = TextureManager::LoadTex(hpFrameTex_);
	hpBarFrame_.reset(Sprite::Create(tex, { 962,58 }, { 962,58 }, { 962,58 }, { 640,60 }));
#pragma endregion


}

void BossEnemy::Initialize()
{
	world_.Initialize();
	world_.scale_ = { 1.0f,1.0f,1.0f };
	world_.translate_.z = 20.0f;
	world_.translate_.y = 2.0f;
	model_->SetUVScale({ 1.0f,1.0f,1.0f });

	world_.UpdateMatrix();

	collider_.reset(new SphereCollider);
	collider_->Initialize("boss", world_);

	enemies_.clear();
	reqBehavior_ = IDOL;
	isActive_ = true;

	invisibleFrame_ = 60;
	isInvisible_ = false;

#pragma region HP関連初期化
	//HPの初期化
	HP_ = maxHP_;
	hpBar_->SetScale(hpBarScale);
#pragma endregion

}

void BossEnemy::Update()
{
	if (!isActive_)
	{
		return;
	}
	if (behavior_ != CRUSH && HP_ <= 0)
	{
		reqBehavior_ = CRUSH;
	}
	enemies_.remove_if([](const std::unique_ptr<SomeEnemy>& some) {
		if (some->GetIsActive())
		{
			return false;
		}
		return true;
		});

	if (reqBehavior_)
	{
		behavior_ = reqBehavior_.value();
		switch (behavior_)
		{
		case BossEnemy::IDOL:
			momentFrame_ = 60;
			break;
		case BossEnemy::SUMMON:
			momentFrame_ = 30;
			break;
		case BossEnemy::DAMAGE:
			momentFrame_ = 30;
			invisibleFrame_ = 60;
			isInvisible_ = true;
			HP_--;
			break;
		case BossEnemy::CRUSH:
			momentFrame_ = 120;
			isInvisible_ = true;
			break;
		default:
			break;
		}
		reqBehavior_ = std::nullopt;
	}

	switch (behavior_)
	{
	case BossEnemy::IDOL:
		UpdateIDOL();
		break;
	case BossEnemy::SUMMON:
		UpdateSUMMON();
		break;
	case BossEnemy::DAMAGE:
		UpdateDAMAGE();
		break;
	case BossEnemy::CRUSH:
		UpdateCRUSH();
		break;
	default:
		break;
	}
	std::list<std::unique_ptr<SomeEnemy>>::iterator itr = enemies_.begin();
	for (; itr != enemies_.end(); ++itr)
	{
		itr->get()->Update();
	}

	if (isInvisible_)
	{
		invisibleFrame_--;
		if (invisibleFrame_ <= 0)
		{
			isInvisible_ = false;
		}
	}

	world_.UpdateMatrix();
	collider_->Update();

	HPBarUpdate();
}

void BossEnemy::DebagWindow()
{
	//model_->DebugParameter("boss");
	collider_->Debug("boss");

	ImGui::Begin("boss");

	ImGui::Text("invisible  : %d", invisibleFrame_);
	ImGui::Text("isInvisible: %s", isInvisible_ ? "TRUE" : "FALSE");
	ImGui::Text("moment     : %d", momentFrame_);
	ImGui::Text("isActive   : %s", isActive_ ? "TRUE" : "FALSE");
	ImGui::DragInt("HP", &HP_);

	ImGui::DragFloat3("position", &world_.translate_.x, 0.01f);

	switch (behavior_)
	{
	case BossEnemy::IDOL:
		ImGui::Text("IDOL");
		break;
	case BossEnemy::SUMMON:
		ImGui::Text("SUMMON");
		break;
	case BossEnemy::DAMAGE:
		ImGui::Text("DAMAGE");
		break;
	case BossEnemy::CRUSH:
		ImGui::Text("CRUSH");
		break;
	default:
		break;
	}
	if (ImGui::Button("Summon"))
	{
		reqBehavior_ = SUMMON;
	}

	std::list<std::unique_ptr<SomeEnemy>>::iterator itr = enemies_.begin();
	int num = 0;
	for (; itr != enemies_.end(); ++itr)
	{
		itr->get()->DebagWindow(num++);
	}

	hpBar_->DrawDebugImGui("HPBar");
	hpBarBack_->DrawDebugImGui("HPBarBack");
	hpBarFrame_->DrawDebugImGui("HPBarFrame");

	ImGui::End();
}

void BossEnemy::Draw(const Matrix4x4& viewp)
{
	if (!isActive_)
	{
		return;
	}
	std::list<std::unique_ptr<SomeEnemy>>::iterator itr = enemies_.begin();
	for (; itr != enemies_.end(); ++itr)
	{
		itr->get()->Draw();
	}
	GameObject::Draw(viewp);
	collider_->Draw();


	hpBarBack_->Draw();
	hpBar_->Draw();
	hpBarFrame_->Draw();
}

void BossEnemy::OnCollision()
{
	if (!isInvisible_)
	{
		reqBehavior_ = DAMAGE;
	}
}

void BossEnemy::SummmonEnemy()
{
	SomeEnemy* data = new SomeEnemy;
	data->Initialize();
	enemies_.emplace_back(data);
	reqBehavior_ = IDOL;
}

void BossEnemy::HPBarUpdate()
{
	//生きているときに処理
	if (isActive_)
	{
		//最大HPと現HPの比率計算
		float hphiritu = (float)HP_ / (float)maxHP_;

		//テクスチャのスケールを計算
		float scaleX = hpBarScale.x * hphiritu;

		//スケール計算
		Vector3 nowScale = hpBarScale;
		nowScale.x = scaleX;

		//変更
		hpBar_->SetScale(nowScale);
	}
}

void BossEnemy::UpdateIDOL()
{
	if (enemies_.size() < 5)
	{
		momentFrame_--;
		if (momentFrame_ <= 0)
		{
			reqBehavior_ = SUMMON;
		}
	}
}

void BossEnemy::UpdateSUMMON()
{
	momentFrame_--;
	if (momentFrame_ <= 0)
	{
		SummmonEnemy();
		reqBehavior_ = IDOL;
	}
}

void BossEnemy::UpdateDAMAGE()
{
	momentFrame_--;
	if (momentFrame_ <= 0)
	{
		reqBehavior_ = IDOL;

	}
}

void BossEnemy::UpdateCRUSH()
{
	world_.scale_ += {0.05f, 0.05f, 0.05f};
	invisibleFrame_ = 10;
	momentFrame_--;
	if (momentFrame_ <= 0)
	{
		isActive_ = false;
	}
}
