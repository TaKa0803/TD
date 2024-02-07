#include "GameScene.h"
#include<imgui.h>
#include<numbers>

#include"Scenes.h"
#include"InstancingModelManager/InstancingModelManager.h"
#include"TextureManager/TextureManager.h"

//#include "SphereCollider/SphereCollider.h"
#include"RandomNum/RandomNum.h"

GameScene::GameScene()
{


	input_ = Input::GetInstance();

	camera_ = std::make_unique<Camera>();

	plane_ = std::make_unique<Plane>();

	player_ = std::make_unique<Player>();

	stage_ = std::make_unique<Stage>();

	boss_ = std::make_unique<BossEnemy>();
	boss_->SetGage(goodGage_, maxGoodGage_);

	skydome_ = std::make_unique<Skydome>();


	float pi = 0;
	Vector3 offset = { 0,0,1 };
	WorldTransform world;

	for (auto& step : steps_)
	{
		step = std::make_unique<Step>();

		world.rotate_.y = pi;
		world.UpdateMatrix();
		Vector3 pos = (TransformNormal(offset, world.matWorld_)) * stepLength_;
		step->SetTranslate(pos);
		step->SetRotate(pi);
		pi += (3.14f * 2.0f) / 5.0f;
	}
	EffectExp_ = EffectExplosion::GetInstance();

	eSExplo_ = EfSphereExplosion::GetInstance();

	dZoneEM_ = DZoneEffect::GetInstance();

#pragma region UI関係
	for (int i = 0; i < kNumSprite; ++i)
	{
		int tex = TextureManager::LoadTex(gageSpritePath[i]);
		gageSprite_[i].reset(Sprite::Create(tex, { 842,117 }, { 842,117 }, { 842,117 }, { 0,0 }, { 0,0.5f }));
		gageSprite_[i]->SetParent(UIWorld_);
	}

	gageSprite_[Good]->SetPosition({ 163,0,0 });
	gageSprite_[Bad]->SetPosition({ 163,0,0 });

	UIWorld_.translate_ = UIPosition_;
	UIWorld_.scale_ = UIScale_;
#pragma endregion


}

GameScene::~GameScene()
{

}

void GameScene::Initialize()
{


	stage_->Initialize();

	plane_->Initialize();

	player_->Initialize();

	boss_->Initialize(player_->GetWorld());
	boss_->SetCamera(*camera_.get());
	//初期化
	camera_->Initialize();
	//各種設定
	camera_->SetTarget(&player_->GetWorld());
	player_->SetCamera(camera_.get());

	camera_->SetCameraDirection(-100);
	skydome_->Initialize();


	for (auto& step : steps_)
	{
		step->Initialize();
	}

	EffectExp_->Initialize();

	eSExplo_->Initialize("Explo");

	dZoneEM_->Initialize();
	//ゲージ初期化
	goodGage_ = 0;
	badGage_ = 0;
}



void GameScene::Update()
{



#pragma region ゲームシーン
	//デバッグウィンドウ表示
	DebugWindows();

	skydome_->Update();

	stage_->Update();


	player_->Update();

	boss_->SetGage(goodGage_, maxGoodGage_);
	boss_->Update();



#pragma region 観客
	for (auto& step : steps_)
	{
		step->Update();
	}
#pragma endregion

	//カメラ更新
	CameraUpdate();

#pragma endregion

	CheckCollision();

	UIUpdate();

	EffectExp_->Update();
	eSExplo_->Update();
	dZoneEM_->Update();


	SceneChange();
}

void GameScene::Draw()
{

	skydome_->Draw();

	player_->Draw(*camera_);
	//地面
	plane_->Draw(*camera_);

	boss_->Draw();

	stage_->Draw();
	//player_->Draw(camera_->GetViewProjectionMatrix());

	EffectExp_->Draw();
	eSExplo_->Draw();
	dZoneEM_->Draw();

#pragma region 観客
	for (auto& step : steps_)
	{
		step->Draw();
	}
#pragma endregion

	//インスタンシングのモデルを全描画
	InstancingModelManager::GetInstance()->DrawAllModel(camera_->GetViewProjectionMatrix());





#pragma region ゲームUI
	UIWorld_.UpdateMatrix();
	for (auto& sprite : gageSprite_)
	{
		sprite->Draw();
	}
#pragma endregion

}

void GameScene::DebugWindows()
{

#ifdef _DEBUG
	//カメラのデバッグ表示
	camera_->DrawDebugWindow("camera");

	player_->DebagWindow();

	plane_->DebagWindow();

	stage_->DebagWindow();

	boss_->DebagWindow();

	ImGui::Begin("dai");
	ImGui::DragFloat("length", &stepLength_);

	ImGui::Checkbox("isCollision", &isCollisionBoss_);
	ImGui::Text("Bad :%d", badGage_);
	ImGui::End();
	float pi = 0;
	Vector3 offset = { 0,0,1 };
	WorldTransform world;

	for (auto& step : steps_)
	{
		step = std::make_unique<Step>();

		world.rotate_.y = pi;
		world.UpdateMatrix();
		Vector3 pos = (TransformNormal(offset, world.matWorld_)) * stepLength_;
		step->SetTranslate(pos);
		step->SetRotate(pi);
		pi += (3.14f * 2.0f) / 5.0f;
	}
	EffectExp_ = EffectExplosion::GetInstance();

#pragma region ゲームUI
	/*
	if (!ImGui::Begin("Sprite", nullptr, ImGuiWindowFlags_MenuBar)) {
		ImGui::End();
	}
	if (ImGui::BeginMenuBar())return;
	*/


	steps_[0]->Debug();


	ImGui::Begin("sprite");
	ImGui::DragFloat3("all Pos", &UIWorld_.translate_.x);
	ImGui::DragFloat3("all scale", &UIWorld_.scale_.x, 0.01f);
	int Index = 0;
	for (auto& sprite : gageSprite_)
	{
		sprite->DrawDebugImGui(gageSpritePath[Index].c_str());
		Index++;
	}
	ImGui::End();
	//ImGui::EndMenuBar();
	//ImGui::End();
#pragma endregion


#endif // _DEBUG


}

void GameScene::CameraUpdate()
{
	Vector2 stick;
	if (input_->IsControllerActive())
	{
		stick = input_->GetjoyStickR();
	}
	else
	{
		Vector3 sti = input_->GetAllArrowKey();
		stick = { sti.x,sti.z };
	}
	stick.Normalize();
	stick.x *= xrotateNum;
	stick.y *= yrotatenum * -1.0f;
	camera_->AddCameraR_X(stick.y);
	camera_->AddCameraR_Y(stick.x);


	camera_->Update();


}

void GameScene::CheckCollision()
{
	// 雑魚敵リスト
	auto& enemies = boss_->GetEnemies();
	auto itrE = enemies.begin();

	// 範囲攻撃リスト
	auto& attacks = boss_->GetAttacks();
	auto itrA = attacks.begin();


	/*std::list<SphereCollider*> eCollider;
	for (; itrE != enemies.end(); ++itrE)
	{
		eCollider.push_back(itrE->get()->GetCollider());
	}*/

	// 音攻撃リスト
	auto& blasts = player_->GetEchoBlasts();
	auto itrB = blasts.begin();

	//std::list<OBBCollider*> bCollider;
	//for (; itrB != blasts.end(); ++itrB)
	//{
	//	bCollider.push_back(itrB->get()->GetCollider());
	//}

	// 壁リスト
	auto& walls = stage_->GetWalls();
	auto itrW = walls.begin();

	Vector3 temp{ 0.0f,0.0f,0.0f };

#pragma region 範囲攻撃とプレイヤー

	// 範囲攻撃とプレイヤー
	itrA = attacks.begin();
	for (; itrA != attacks.end(); ++itrA)
	{
		AreaAttack* attack = itrA->get();
		if (player_->GetCollider()->IsCollision(*attack->GetCollider(), temp))
		{
			if (player_->OnCollision())
			{
				AddBadGage();
			}
		}
	}

#pragma endregion

#pragma region 雑魚敵とプレイヤーの攻撃

	//雑魚敵関係
	itrE = enemies.begin();
	for (; itrE != enemies.end(); ++itrE)
	{
		SomeEnemy* some = itrE->get();
		// 弾かれてない時
		if (!itrE->get()->GetIsBurst())
		{
			// 攻撃との接触
			itrB = blasts.begin();
			for (; itrB != blasts.end(); ++itrB)
			{
				EchoBlast* echo = itrB->get();
				if (echo->GetIsSpot())
				{
					if (some->GetCollider()->IsCollision(*echo->GetCollider(), temp, 3))
					{
						echo->OnCollision();
						some->OnCollision(echo->GetDirection());
					}
				}
			}
		}
		else
		{
			//プレイヤー反射板か否か
			itrB = blasts.begin();
			for (; itrB != blasts.end(); ++itrB)
			{
				EchoBlast* echo = itrB->get();
				if (!echo->GetIsSpot())
				{
					if (some->GetCollider()->IsCollision(*echo->GetCollider(), temp, 3))
					{
						echo->OnCollision();
						some->OnCollision(echo->GetDirection());
					}
				}
			}
		}
	}

#pragma endregion

#pragma region 雑魚敵とボス

	itrE = enemies.begin();
	for (; itrE != enemies.end(); ++itrE)
	{
		SomeEnemy* some = itrE->get();
		// こわれている時
		if (itrE->get()->GetIsDestroy())
		{
			BossEnemy* boss = boss_.get();
			// ボスとの接触
			if (some->GetCollider()->IsCollision(*boss->GetCollider(), temp))
			{
				if (some->OnCollision())
				{
					boss->OnCollision(some->GetAttackPower());

					//エフェクト発生
					AddEffect(some->GetWorld());

					//Goodゲージ増加
					AddGoodGage(some->GetAttackPower());
				}
			}

			//プレイヤーと爆破
			if (some->GetCollider()->IsCollision(*player_->GetCollider(), temp))
			{
				// プレイヤーが衝突判定を取れる時
				if (player_->OnCollision())
				{
					AddBadGage();
					some->OnCollision();

					//エフェクト発生
					AddEffect(some->GetWorld());

					//some->OnCollision(temp);
				}
			}
		}
		// 弾かれてる間の判定
		else if (itrE->get()->GetIsBurst())
		{

			//プレイヤーと飛ばされている敵の判定
			if (some->GetCollider()->IsCollision(*player_->GetCollider(), temp))
			{
				// プレイヤーが衝突判定を取れる時
				if (player_->OnCollision())
				{
					AddBadGage();
					some->OnCollision();

					//エフェクト発生
					AddEffect(some->GetWorld());
				}
			}

			// ボスとの接触
			if (isCollisionBoss_)
			{
				BossEnemy* boss = boss_.get();
				if (some->GetCollider()->IsCollision(*boss->GetCollider(), temp))
				{
					if (some->OnCollision())
					{
						boss->OnCollision(some->GetAttackPower());

						//エフェクト発生
						AddEffect(some->GetWorld());

						//Goodゲージ増加
						AddGoodGage(some->GetAttackPower());
					}
				}
			}


			// 敵同士の衝突
			auto itrE2 = enemies.begin();
			for (; itrE2 != enemies.end(); ++itrE2)
			{
				SomeEnemy* some2 = itrE2->get();
				if (!some2->GetCanEnemy())
				{
					continue;
				}
				if (some->GetCollider()->IsCollision(*some2->GetCollider(), temp))
				{
					/*Vector3 direct = some->GetDirection();
					direct.x += RandomNumber::Get(-0.6f, 0.6f);
					direct.z += RandomNumber::Get(-0.6f, 0.6f);
					some2->OnEnemy(direct);*/
					some2->OnEnemy(-temp);
				}
			}
		}
	}

#pragma endregion

#pragma region 雑魚敵と壁

	//壁関係処理
	itrW = walls.begin();
	for (; itrW != walls.end(); ++itrW)
	{
		StageWall* wall = itrW->get();
		//プレイヤーと壁の当たり判定
		if (player_->GetCollider()->IsCollision(*wall->GetCollider(), temp, 1))
		{
			wall->OnCollision();
			player_->BackVector(temp);
		}

		//雑魚敵と壁の判定
		itrE = enemies.begin();
		for (; itrE != enemies.end(); ++itrE)
		{
			SomeEnemy* some = itrE->get();
			if (!some->GetIsBurst())
			{
				continue;
			}
			if (some->GetCollider()->IsCollision(*wall->GetCollider(), temp, 10))
			{
				wall->OnCollision();

				Vector3 direc = wall->GetDirection();

				direc.SetNormalize();
				direc *= Length(temp);

				some->OnCollision(direc);
			}
		}

		//スペシャル攻撃の判定
		if (boss_->GetSpecialATKCollider()->IsCollision(*wall->GetCollider(), temp, 1))
		{

			wall->OnCollision();

			//押し返しベクトル計算
			Vector3 direc = wall->GetDirection();
			direc.SetNormalize();
			direc *= Length(temp);
			boss_->SPATKOnCollison(direc);
		}

		//カメラとの判定
		camera_->IsCollision(wall->GetCollider());

		/*
		//透明化処理
		OBBCollider* obbc = wall->GetCollider();
		if (obbc->IsCollision(camera_->GetSegment())) {
			wall->SetColor(true);
		}
		else {
			wall->SetColor(false);
		}
		*/
	}

#pragma endregion

#pragma region ボスの必殺技とプレイヤーの攻撃

	//ボスの必殺技との処理
	if (boss_->IsSpecialAttackActive())
	{
		for (; itrB != blasts.end(); ++itrB)
		{
			EchoBlast* echo = itrB->get();
			if (!echo->GetIsSpot() && boss_->GetSpecialATKCollider()->IsCollision(*echo->GetCollider(), temp, 3))
			{
				//押し返しベクトル計算
				Vector3 direc = echo->GetDirection();
				direc.SetNormalize();
				direc *= Length(temp);
				boss_->SPATKReflectOnCollision(direc);
			}
		}
	}
#pragma endregion

#pragma region ボスの必殺技とプレイヤー

	//プレイヤーとスペシャル攻撃との判定
	//反射板なしでプレイヤーヒットで爆破
	if (!boss_->IsHitPlayerReflection() && boss_->IsSpecialAttackActive() && player_->GetCollider()->IsCollision(*boss_->GetSpecialATKCollider(), temp))
	{
		// プレイヤーが衝突判定を取れる時
		if (player_->OnCollision())
		{
			AddBadGage();
			//爆発
			boss_->SPATKOnColliExplo();
		}
	}

#pragma endregion

#pragma region ボスの必殺技とボス

	if (boss_->IsSpecialAttackActive())
	{

		if (boss_->IsHitPlayerReflection())
		{
			boss_->SetAplta(1.0f);
			if (boss_->GetSpecialATKCollider()->IsCollision(*boss_->GetCollider(), temp))
			{
				boss_->SPATKOnColliExplo();
				boss_->OnCollision(10);
				AddGoodGage(10);

			}
		}
		else
		{
			boss_->SetAplta(0.5f);
		}
	}
	else
	{
		boss_->SetAplta(1.0f);
	}


#pragma endregion


}


void GameScene::SceneChange()
{
	//ボスが死んでたら
	if (!boss_->GetIsActive())
	{
		sceneNo = SCENE::CLEAR;
	}

	//BADゲージがいっぱいになったら
	if (badGage_ == maxBadGage_)
	{
		sceneNo = FAIL;
	}

}

void GameScene::UIUpdate()
{
	//比率計算
	float scaleX = goodGage_ / float(maxGoodGage_);
	//比率に合わせたサイズ取得
	scaleX *= barScale_.x;
	//Vector3型にする
	Vector3 scale = barScale_;
	scale.x = scaleX;
	//入れる
	gageSprite_[Good]->SetScale(scale);


	scaleX = badGage_ / float(maxBadGage_);

	scaleX *= barScale_.x;

	scale.x = scaleX;

	gageSprite_[Bad]->SetScale(scale);


}

void GameScene::AddEffect(const WorldTransform& spawnW)
{
#pragma region エフェクト出現
	EffectData newData;

	//エフェクトに使うモデルのタグ設定
	newData.tag = eTag_;

	//数分生成
	for (int i = 0; i < 10; ++i)
	{
		moveData movedata;
		//データ設定
		movedata.world = spawnW;
		movedata.world.scale_ = { 0.2f,0.2f ,0.2f };
		//初期速度ランダム
		movedata.velo = {
			RandomNumber::Get(-1,1),
			RandomNumber::Get(-1,1),
			RandomNumber::Get(-1,1)
		};

		movedata.velo.SetNormalize();
		movedata.velo *= 1.0f;
		//加速度設定
		movedata.acce = { 0,-0.1f,0 };
		//死亡までのカウント
		movedata.maxDeadCount = 60;

		//データ設定
		newData.mData.emplace_back(movedata);

	}
	//各粒データを含めた総合データ送信
	EffectExp_->AddEffectData(newData);
#pragma endregion
}

void GameScene::AddGoodGage(float num)
{
	goodGage_ += num;

	if (goodGage_ > maxGoodGage_)
	{
		goodGage_ = maxGoodGage_;
	}
}

void GameScene::AddBadGage()
{
	badGage_++;
	if (badGage_ > maxBadGage_)
	{
		badGage_ = maxBadGage_;
	}
}
