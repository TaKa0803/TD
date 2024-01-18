#include "GameScene.h"
#include<imgui.h>

#include"Scenes.h"
#include"InstancingModelManager/InstancingModelManager.h"
#include"TextureManager/TextureManager.h"

GameScene::GameScene()
{
	sceneNo = GAME;


	input_ = Input::GetInstance();

	camera_ = std::make_unique<Camera>();

	plane_ = std::make_unique<Plane>();

	player_ = std::make_unique<Player>();

	stage_ = std::make_unique<Stage>();
}

GameScene::~GameScene()
{

}

void GameScene::Initialize()
{

	
	stage_->Initialize();

	plane_->Initialize();

	player_->Initialize();

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

	//カメラ更新
	CameraUpdate();

	stage_->Update();

	player_->Update();
#pragma endregion



}

void GameScene::Draw()
{

	player_->Draw(camera_->GetViewProjectionMatrix());
	//地面
	plane_->Draw(camera_->GetViewProjectionMatrix());

	//player_->Draw(camera_->GetViewProjectionMatrix());

	stage_->Draw(camera_->GetViewProjectionMatrix());

	//インスタンシングのモデルを全描画
	InstancingModelManager::GetInstance()->DrawAllModel(camera_->GetViewProjectionMatrix());


}

void GameScene::DebugWindows()
{

#ifdef _DEBUG
	//カメラのデバッグ表示
	camera_->DrawDebugWindow("camera");

	//player_->DebagWindow();

	plane_->DebagWindow();

	stage_->DebagWindow();

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
