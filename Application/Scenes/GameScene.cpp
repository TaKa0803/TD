#include "GameScene.h"
#include<imgui.h>

#include"Scenes.h"
#include"InstancingModelManager/InstancingModelManager.h"
#include"TextureManager/TextureManager.h"

#include "SphereCollider/SphereCollider.h"

GameScene::GameScene()
{


	input_ = Input::GetInstance();

	camera_ = std::make_unique<Camera>();

	plane_ = std::make_unique<Plane>();

	player_ = std::make_unique<Player>();

	stage_ = std::make_unique<Stage>();

	boss_ = std::make_unique<BossEnemy>();
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

}



void GameScene::Update()
{


#pragma region ゲームシーン
	//デバッグウィンドウ表示
	DebugWindows();

	stage_->Update();

	player_->Update();

	boss_->Update();

	//カメラ更新
	CameraUpdate();
#pragma endregion

	CheckCollision();

}

void GameScene::Draw()
{

	player_->Draw(camera_->GetViewProjectionMatrix());
	//地面
	plane_->Draw(camera_->GetViewProjectionMatrix());

	boss_->Draw(camera_->GetViewProjectionMatrix());

	//player_->Draw(camera_->GetViewProjectionMatrix());

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

	Vector3 temp{ 0.0f,0.0f,0.0f };

	itrB = blasts.begin();
	for (; itrB != blasts.end(); ++itrB)
	{
		EchoBlast* echo = itrB->get();
		itrE = enemies.begin();
		for (; itrE != enemies.end(); ++itrE)
		{
			SomeEnemy* some = itrE->get();
			if (some->GetCollider()->IsCollision(*echo->GetCollider(), temp))
			{
				echo->OnCollision();
				some->OnCollision(echo->GetDirection());
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
