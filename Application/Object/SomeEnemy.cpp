#include "SomeEnemy.h"

#include "ImGuiManager/ImGuiManager.h"

void SomeEnemy::Initialize(const WorldTransform& world)
{
	InstancingGameObject::Initialize("parrot");
	world_.Initialize();
	world_.scale_ = { 1.0f,1.0f,1.0f };
	world_.translate_.y = 2.0f;
	world_.UpdateMatrix();

	collider_.reset(new SphereCollider);
	collider_->Initialize("enemy");

	isActive_ = true;
	isHit_ = false;

	playerW_ = &world;

	//初期化
	reqBehavior_ = IDOL;
}

void SomeEnemy::Update()
{
	//死んでいたら終わる
	if (!isActive_){
		return;
	}

	//各タイプによる行動処理
	switch (eType_)
	{
	case SomeEnemy::Move:
		MoveEnemyUpdate();
		break;
	case SomeEnemy::Explo:
		ExpEnemyUpdate();
		break;
	default:
		break;
	}

	

	

	world_.UpdateMatrix();
	collider_->SetTranslate(world_.translate_);
	collider_->Update();
}

void SomeEnemy::DebagWindow(int num)
{
	collider_->Debug("enemy");

	ImGui::Begin("SomeEnemy");

	ImGui::DragFloat3(std::to_string(num).c_str(), &world_.translate_.x, 0.02f);

	ImGui::End();
}

void SomeEnemy::Draw()
{
	if (!isActive_)
	{
		return;
	}
	collider_->Draw();
	IMM_->SetData(tag_, world_);
}

void SomeEnemy::OnCollision(const Vector3& direction)
{
	if (behavior_ == BURST)
	{
		direct3_ = direction;
	}
	
	//動くタイプの時
	if (eType_==Move)
	{
		direct3_ = direction;
		reqBehavior_ = BURST;
	}//即爆タイプの時
	else if (eType_==Explo)
	{
		reqBehavior_ = DESTROY;
	}
}

void SomeEnemy::MoveEnemyUpdate()
{
#pragma region 移動する敵
	if (reqBehavior_)
	{
		behavior_ = reqBehavior_.value();
		switch (behavior_)
		{
		case SomeEnemy::IDOL:
			momentFrame_ = cALIVEFRAME_;
			break;
		case SomeEnemy::MOVE:
			momentFrame_ = cALIVEFRAME_;
			break;
		case SomeEnemy::BURST:
			momentFrame_ = cBURSTFRAME_;
			break;
		case SomeEnemy::DESTROY:
			momentFrame_ = 60;
			break;
		default:
			break;
		}
		reqBehavior_ = std::nullopt;
	}

	switch (behavior_)
	{
	case SomeEnemy::IDOL:
		momentFrame_--;
		if (momentFrame_ <= 0)
		{
			isActive_ = false;
		}
		break;
	case SomeEnemy::MOVE:
		momentFrame_--;
		if (momentFrame_ <= 0)
		{
			isActive_ = false;
		}
		break;
	case SomeEnemy::BURST:
		momentFrame_--;
		if (momentFrame_ <= 0)
		{
			reqBehavior_ = DESTROY;
		}
		world_.translate_ += direct3_ * 0.5f;
		break;
	case SomeEnemy::DESTROY:
		momentFrame_--;
		if (momentFrame_ <= 0)
		{
			isActive_ = false;
		}
		collider_->SetRadius(1.5f + (1.0f - momentFrame_ / 60.0f));
		break;
	default:
		break;
	}


#pragma endregion
}

void SomeEnemy::ExpEnemyUpdate()
{
#pragma region 爆発する敵
	if (reqBehavior_)
	{
		behavior_ = reqBehavior_.value();
		switch (behavior_)
		{
		case SomeEnemy::IDOL:
			momentFrame_ = cALIVEFRAME_;
			break;
		case SomeEnemy::MOVE:
			momentFrame_ = cALIVEFRAME_;
			break;
		case SomeEnemy::BURST:
			momentFrame_ = cBURSTFRAME_;
			break;
		case SomeEnemy::DESTROY:
			momentFrame_ = 60;
			break;
		default:
			break;
		}
		reqBehavior_ = std::nullopt;
	}

	if (behavior_ != BURST) {
		momentFrame_--;
		if (momentFrame_ <= 0)
		{
			isActive_ = false;
		}
	}

	switch (behavior_)
	{
	case SomeEnemy::IDOL:

		break;
	case SomeEnemy::MOVE:

		break;
	case SomeEnemy::BURST:
		if (momentFrame_ <= 0)
		{
			reqBehavior_ = DESTROY;
		}
		world_.translate_ += direct3_ * 0.5f;
		break;
	case SomeEnemy::DESTROY:

		collider_->SetRadius(1.5f + (1.0f - momentFrame_ / 60.0f));
		break;
	default:
		break;
	}

#pragma endregion

}

