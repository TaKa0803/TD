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
	std::list<SphereCollider*> eCollider;
	for (; itrE != enemies.end(); ++itrE)
	{
		eCollider.push_back(itrE->get()->GetCollider());
	}
	auto& blasts = player_->GetEchoBlasts();
	auto itrB = blasts.begin();
	std::list<OBBCollider*> bCollider;
	for (; itrB != blasts.end(); ++itrB)
	{
		bCollider.push_back(itrB->get()->GetCollider());
	}

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
	}
	itrE = enemies.begin();
	for (; itrE != enemies.end(); ++itrE)
	{
		SomeEnemy* some = itrE->get();
	
		BossEnemy* boss = boss_.get();
		if (some->GetCollider()->IsCollision(*boss->GetCollider(), temp))
		{
			some->OnCollision();
			boss->OnCollision();

#pragma region エフェクト出現
			EffectData newData;

			newData.tag = eTag_;

			for (int i = 0; i < 10; ++i) {

				moveData movedata;

				movedata.world = { 0,2,0 };
				movedata.world.scale_ = { 0.2f,0.2f ,0.2f };
				movedata.velo = {
					RandomNumber::Get(-1,1),
					RandomNumber::Get(-1,1),
					RandomNumber::Get(-1,1)
				};

				movedata.velo.SetNormalize();
				movedata.velo *= 1.0f;

				movedata.acce = { 0,-0.1f,0 };

				movedata.maxDeadCount = 60;

				newData.mData.push_back(movedata);

			}
			EffectExp_->AddEffectData(newData);
#pragma endregion
		}

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
