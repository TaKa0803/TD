#include "DebugScene.h"
#include"Scenes.h"
#include"InstancingModelManager/InstancingModelManager.h"

DebugScene::DebugScene()
{
	input_ = Input::GetInstance();

	camera_ = std::make_unique<Camera>();
	sceneNo = DEBUG;

	sphere_ = std::make_unique<SphereCollider>();
	plane_ = std::make_unique<PlaneCollider>();
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
}

void DebugScene::Draw()
{
	sphere_->Draw();
	plane_->Draw();
	obb_->Draw();
	InstancingModelManager::GetInstance()->DrawAllModel(camera_->GetViewProjectionMatrix());
}

void DebugScene::Debug()
{
	sphere_->Debug("sphere");
	camera_->DrawDebugWindow("camera");
	obb_->Draw();
}
