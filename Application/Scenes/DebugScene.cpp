#include "DebugScene.h"
#include"Scenes.h"
#include"InstancingModelManager/InstancingModelManager.h"
#include"RandomNum/RandomNum.h"

DebugScene::DebugScene()
{
	input_ = Input::GetInstance();

	camera_ = std::make_unique<Camera>();


	sphere_ = std::make_unique<SphereCollider>();
	plane_ = std::make_unique<PlaneCollider>();

	//エフェクト
	EffectExp_ = EffectExplosion::GetInstance();
}

DebugScene::~DebugScene(){}


void DebugScene::Initialize()
{

	cWorld_.Initialize();
	camera_->Initialize();
	camera_->SetTarget(&cWorld_);

	sWorld_.Initialize();

	sphere_ = std::make_unique<SphereCollider>();
	sphere_->Initialize("sphere",sWorld_);

	pWorld_.Initialize();
	plane_ = std::make_unique<PlaneCollider>();
	plane_->Initialize("plane", pWorld_);

	oWorld_.Initialize();
	obb_ = std::make_unique<OBBCollider>();
	obb_->Initialize("plane", oWorld_);

	EffectExp_->Initialize();
}

void DebugScene::Update()
{
	Debug();

	camera_->Update();


	Vector3 move = input_->GetAllArrowKey();

	sWorld_.translate_ += move;
	sWorld_.UpdateMatrix();

	sphere_->Update();
	plane_->Update();
	obb_->Update();

	
	Vector3 backV;
	if (obb_->IsCollision(sphere_.get(),backV)) {
		oWorld_.translate_ += backV;
		oWorld_.UpdateMatrix();
		obb_->Update();
	}


	if (input_->TriggerKey(DIK_SPACE)) {

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
	}

	EffectExp_->Update();

}

void DebugScene::Draw()
{
	sphere_->Draw();
	plane_->Draw();
	obb_->Draw();

	EffectExp_->Draw();

	InstancingModelManager::GetInstance()->DrawAllModel(camera_->GetViewProjectionMatrix());
}

void DebugScene::Debug()
{
	sWorld_.DrawDebug("sphere");
	sphere_->Debug("sphere");
	camera_->DrawDebugWindow("camera");
	obb_->Debug("box");
}
