#include "GameScene.h"
#include<imgui.h>

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

	skydome_ = std::make_unique<Skydome>();

	EffectExp_ = EffectExplosion::GetInstance();


#pragma region UI関係
	for (int i = 0; i < kNumSprite; ++i) {
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

	boss_->Initialize();

	//初期化
	camera_->Initialize();
	//各種設定
	camera_->SetTarget(&player_->GetWorld());
	player_->SetCamera(camera_.get());

	camera_->SetCameraDirection(-100);
	skydome_->Initialize();

	EffectExp_->Initialize();

	//ゲージ初期化
	goodGage_ = 0;
	badGage_ = 0;
}



void GameScene::Update()
{

	if (!boss_->GetIsActive())
	{
		sceneNo = SCENE::CLEAR;
	}

#pragma region ゲームシーン
	//デバッグウィンドウ表示
	DebugWindows();

	skydome_->Update();

	stage_->Update();

	player_->Update();

	boss_->Update();

	//カメラ更新
	CameraUpdate();
#pragma endregion

	CheckCollision();

	EffectExp_->Update();

	UIUpdate();
}

void GameScene::Draw()
{

	skydome_->Draw();

	player_->Draw(camera_->GetViewProjectionMatrix());
	//地面
	plane_->Draw(camera_->GetViewProjectionMatrix());

	boss_->Draw(camera_->GetViewProjectionMatrix());

	stage_->Draw();
	//player_->Draw(camera_->GetViewProjectionMatrix());

	EffectExp_->Draw();

	//インスタンシングのモデルを全描画
	InstancingModelManager::GetInstance()->DrawAllModel(camera_->GetViewProjectionMatrix());


#pragma region ゲームUI
	UIWorld_.UpdateMatrix();
	for (auto& sprite : gageSprite_) {
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

#pragma region ゲームUI
	/*
	if (!ImGui::Begin("Sprite", nullptr, ImGuiWindowFlags_MenuBar)) {
		ImGui::End();
	}
	if (ImGui::BeginMenuBar())return;
	*/

	ImGui::Begin("sprite");
	ImGui::DragFloat3("all Pos", &UIWorld_.translate_.x);
	ImGui::DragFloat3("all scale", &UIWorld_.scale_.x,0.01f);
	int Index = 0;
	for (auto& sprite : gageSprite_) {
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
	auto& enemies = boss_->GetEnemies();
	auto itrE = enemies.begin();
	
	/*std::list<SphereCollider*> eCollider;
	for (; itrE != enemies.end(); ++itrE)
	{
		eCollider.push_back(itrE->get()->GetCollider());
	}*/

	auto& blasts = player_->GetEchoBlasts();
	auto itrB = blasts.begin();

	//std::list<OBBCollider*> bCollider;
	//for (; itrB != blasts.end(); ++itrB)
	//{
	//	bCollider.push_back(itrB->get()->GetCollider());
	//}

	auto& walls = stage_->GetWalls();
	auto itrW = walls.begin();

	Vector3 temp{ 0.0f,0.0f,0.0f };

	itrW = walls.begin();
	for (; itrW != walls.end(); ++itrW)
	{
		StageWall* wall = itrW->get();
		if (player_->GetCollider()->IsCollision(*wall->GetCollider(), temp))
		{
			wall->OnCollision();
			player_->BackVector(temp);
		}
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
				if (!some->OnCollision())
				{
					break;
				}
				boss->OnCollision();

#pragma region エフェクト出現
				EffectData newData;

				//エフェクトに使うモデルのタグ設定
				newData.tag = eTag_;

				//数分生成
				for (int i = 0; i < 10; ++i)
				{
					moveData movedata;
					//データ設定
					movedata.world = some->GetWorld();
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
		}
		
		// 弾かれてる間の判定
		if (itrE->get()->GetIsBurst())
		{
			BossEnemy* boss = boss_.get();
			// ボスとの接触
			if (some->GetCollider()->IsCollision(*boss->GetCollider(), temp))
			{
				some->OnCollision();
				boss->OnCollision();

#pragma region エフェクト出現
				EffectData newData;

				//エフェクトに使うモデルのタグ設定
				newData.tag = eTag_;

				//数分生成
				for (int i = 0; i < 10; ++i)
				{
					moveData movedata;
					//データ設定
					movedata.world = some->GetWorld();
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

			// 壁との接触
			itrW = walls.begin();
			for (; itrW != walls.end(); ++itrW)
			{
				StageWall* wall = itrW->get();
				if (some->GetCollider()->IsCollision(*wall->GetCollider(), temp))
				{
					wall->OnCollision();
					some->OnCollision(wall->GetDirection());
				}

			}
		}
		// 弾かれてない時
		else
		{
			// 攻撃との接触
			itrB = blasts.begin();
			for (; itrB != blasts.end(); ++itrB)
			{
				EchoBlast* echo = itrB->get();
				if (some->GetCollider()->IsCollision(*echo->GetCollider(), temp))
				{
					echo->OnCollision();
					some->OnCollision(echo->GetDirection());



				}
			}
		}

	}

	//std::list<SphereCollider*>::iterator blast = bCollider.begin();
	//for (; blast != bCollider.end(); ++blast)
	//{
	//	SphereCollider* sb = (*blast);
	//	std::list<SphereCollider*>::iterator enemy = eCollider.begin();
	//	for (; enemy != eCollider.end(); enemy++)
	//	{
	//		SphereCollider& se = *(*enemy);
	//		if (sb->IsHit(se, temp))
	//		{
	//			
	//		}
	//	}
	//}


}

void GameScene::UIUpdate()
{
	float scaleX = float(goodGage_ / maxGoodGage_);

	scaleX *= barScale_.x;

	Vector3 scale = barScale_;
	scale.x = scaleX;

	gageSprite_[Good]->SetScale(scale);

	scaleX = float(badGage_ / maxBadGage_);

	scaleX *= barScale_.x;

	scale.x = scaleX;

	gageSprite_[Bad]->SetScale(scale);


}
