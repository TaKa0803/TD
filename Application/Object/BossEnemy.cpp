#include "BossEnemy.h"
#include <random>
#include <time.h>

#include "GlobalVariables/GlobalVariables.h"
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


	hpBar_->SetParent(uiWorld_);
	hpBarBack_->SetParent(uiWorld_);
	hpBarFrame_->SetParent(uiWorld_);

#pragma endregion

	srand((unsigned int)time(nullptr));
}

void BossEnemy::Initialize(const WorldTransform& player)
{
	world_.Initialize();
	world_.scale_ = { cSIZE_,cSIZE_,cSIZE_ };
	world_.translate_.z = 20.0f;
	world_.translate_.y = 2.0f;
	model_->SetUVScale({ 1.0f,1.0f,1.0f });

	world_.UpdateMatrix();

	collider_.reset(new SphereCollider);
	collider_->Initialize("boss", world_);
	collider_->SetRadius(cSIZE_);

	enemies_.clear();
	reqBehavior_ = IDOL;
	isActive_ = true;

	invisibleFrame_ = 60;
	isInvisible_ = false;

	damage_ = 0.0f;

	// 範囲取得
	moveLength_ = GlobalVariables::GetInstance()->GetFloatvalue("StageWall", "Size");
	moveCount_ = 0;

#pragma region HP関連初期化
	//HPの初期化
	HP_ = maxHP_;
	hpBar_->SetScale(hpBarScale);

	uiWorld_.translate_ = uiPos_;
	uiWorld_.scale_ = UIScale_;
#pragma endregion
	//プレイヤーworld取得
	playerW_ = &player;

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
		case BossEnemy::MOVE:
		{
			int length = (int)moveLength_;
			int rnd1 = length / 2 - rand() % length;
			int rnd2 = length / 2 - rand() % length;
			nextPosition_ = { (float)rnd1,(float)rnd2 };
		}
		momentFrame_ = cMOVEFRAME_;
		moveCount_++;
		break;
		case BossEnemy::SUMMON:
			momentFrame_ = cSUMMONFRAME_;
			// 移動期待値 0
			moveCount_ = 0;
			break;
		case BossEnemy::DAMAGE:
			momentFrame_ = 30;
			invisibleFrame_ = 60;
			isInvisible_ = true;
			HP_ -= (int)damage_;
			damage_ = 0.0f;
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
	case BossEnemy::MOVE:
		UpdateMOVE();
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
#ifdef _DEBUG

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
	case BossEnemy::MOVE:
		ImGui::Text("MOVE");
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

	ImGui::Begin("Boss UI");
	ImGui::DragFloat3("ui pos", &uiWorld_.translate_.x);
	ImGui::DragFloat3("ui scale", &uiWorld_.scale_.x, 0.01f);
	
	
	ImGui::End();



	ImGui::End();


#endif // _DEBUG

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

void BossEnemy::OnCollision(float damage)
{

	if (!isInvisible_)
	{
		damage_ = damage;
		reqBehavior_ = DAMAGE;
	}
}

void BossEnemy::SummmonEnemy()
{
	SomeEnemy* data = new SomeEnemy;
	data->Initialize(*playerW_);
	if (rand() % 3 == 0)
	{
		data->SetBehavior(SomeEnemy::Explo);
	}
	else
	{
		data->SetBehavior(SomeEnemy::Move);
	}
	int length = (int)moveLength_;
	int rnd1 = length / 2 - rand() % length;
	int rnd2 = length / 2 - rand() % length;
	data->SetPosition({ (float)rnd1,world_.translate_.y,(float)rnd2 });
	enemies_.emplace_back(data);
	reqBehavior_ = IDOL;
}

void BossEnemy::HPBarUpdate()
{
	//生きているときに処理
	if (isActive_)
	{
		uiWorld_.UpdateMatrix();


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
	momentFrame_--;
	if (momentFrame_ <= 0)
	{
		int rnd = rand() % 5;
		// 移動
		if (rnd == 0)
		{
			rnd = rand() % (2 + moveCount_);
			if (rnd == 0)
			{
				reqBehavior_ = MOVE;
			}
			else if (enemies_.size() < 10)
			{
				reqBehavior_ = SUMMON;
			}
			else
			{
				reqBehavior_ = IDOL;
			}
		}
		// 雑魚敵生成
		else //if (rnd == 1)
		{
			if (enemies_.size() < 10)
			{
				reqBehavior_ = SUMMON;
			}
			else
			{
				reqBehavior_ = IDOL;
			}
		}
	}
}

void BossEnemy::UpdateMOVE()
{
	momentFrame_--;
	if (momentFrame_ == cMOVEFRAME_ / 2)
	{
		world_.translate_.x = nextPosition_.x;
		world_.translate_.z = nextPosition_.y;
	}
	else if (momentFrame_ <= 0)
	{
		reqBehavior_ = IDOL;
	}
}

void BossEnemy::UpdateSUMMON()
{
	momentFrame_--;
	if (momentFrame_ == cSUMMONFRAME_ / 2)
	{
		SummmonEnemy();
		SummmonEnemy();
		SummmonEnemy();
	}
	if (momentFrame_ <= 0)
	{
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
