#include "GEParticle.h"
#include"Effect/EffectExplosion.h"
#include"InstancingModelManager/InstancingModelManager.h"
#include"RandomNum/RandomNum.h"

#include<imgui.h>
GEEffectScene::GEEffectScene()
{
	camera_ = std::make_unique<Camera>();
	input_ = Input::GetInstance();
}

GEEffectScene::~GEEffectScene()
{
}

void GEEffectScene::Initialize()
{
	camera_->Initialize();
	camera_->SetTarget(&effectSpawn_);

	EffectExplosion::GetInstance()->Initialize();
}

void GEEffectScene::Update()
{

	Vector3 rotata = input_->GetAllArrowKey();
	
	rotata.x *= (1.0f / 60.0f) * 3.14f;
	camera_->AddCameraR_Y(rotata.x);
	camera_->Update();



#ifdef _DEBUG
	ImGui::Begin("Effect Data");
	ImGui::Text("Space to Spawn");
	ImGui::Text("Left Right Key  move Camera");
	ImGui::DragFloat3("Explo pos", &effectSpawn_.translate_.x, 0.1f);
	ImGui::DragFloat3("velo min", &velomin.x, 0.01f);
	ImGui::DragFloat3("velo max", &velomax.x, 0.01f);
	ImGui::DragFloat3("velo acce", &acce.x, 0.01f);
	ImGui::DragInt("dead Count", & deadCount);
	ImGui::End();
#endif // _DEBUG


	if (input_->TriggerKey(DIK_SPACE) || input_->IsTriggerButton(kButtonB)) {
		EffectData data;

		data.tag = "sphere";
		//各粒データ
		for (int i = 0; i < tubuNum_; i++) {
			moveData mdata;
			mdata.world=effectSpawn_;
			mdata.velo=RandomNumber::Get(velomin,velomax);
			mdata.acce=acce;
			mdata.maxDeadCount=deadCount;
			

			data.mData.emplace_back(mdata);
		}
		EffectExplosion::GetInstance()->AddEffectData(data);
	}

	EffectExplosion::GetInstance()->Update();
}

void GEEffectScene::Draw()
{

	EffectExplosion::GetInstance()->Draw();
	InstancingModelManager::GetInstance()->DrawAllModel(camera_->GetViewProjectionMatrix());
}
