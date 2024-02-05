#include "GECollider.h"
#include<imgui.h>

GEColliderScene::GEColliderScene()
{

	input_ = Input::GetInstance();
	camera_ = std::make_unique<Camera>();
	plane_ = std::make_unique<Plane>();
	wall = std::make_unique<OBBCollider>();
	sphere = std::make_unique<SphereCollider>();

}

GEColliderScene::~GEColliderScene()
{
}

void GEColliderScene::Initialize()
{
	camera_->Initialize();
	plane_->Initialize();
	
	oWorld_.Initialize();
	oWorld_.translate_ = { 0,0,10 };
	oWorld_.UpdateMatrix();	
	wall->Initialize("box",oWorld_);
	
	sWorld_.Initialize();
	sphere->Initialize("sphere",sWorld_);

	camera_->SetTarget(&sWorld_);
}

void GEColliderScene::Update()
{
#ifdef _DEBUG
	ImGui::Begin("guide");
	ImGui::Text("WASD to move Sphere");
	ImGui::DragFloat("move Spd", &moveSpd_);
	ImGui::End();
#endif // _DEBUG

	camera_->DrawDebugWindow("camera");
	wall->Debug("box");
	sphere->Debug("sphere");
	sWorld_.DrawDebug("sphere");

	Vector3 velo = input_->GetWASD();
	velo.SetNormalize();
	velo *= moveSpd_;
	sWorld_.translate_ += velo;
	sWorld_.UpdateMatrix();

	



	//oWorld_.UpdateMatrix();
	plane_->Update();
	wall->Update();
	sphere->Update();

	Vector3 vec;
	if (sphere->IsCollision(*wall.get(), vec, moveSpd_)) {
		
			sWorld_.translate_ += vec;
			sWorld_.UpdateMatrix();
			sphere->Update();
		
	}

	camera_->Update();
}

void GEColliderScene::Draw()
{
	plane_->Draw(*camera_);
	wall->Draw();
	sphere->Draw();

	InstancingModelManager::GetInstance()->DrawAllModel(camera_->GetViewProjectionMatrix());
}
