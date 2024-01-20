#include "StageWall.h"
#include <imgui.h>

void StageWall::SaveNewData()
{
	global_->CreateGroup(cGLOUP);
	//global_->SetValue(cGLOUP, cSIZE, vSize_);
	//global_->SetValue(cGLOUP, cMODELSCALE, vModelScale_);
	//global_->SetValue(cGLOUP, cMODIFIERSCALE, vModifierScale_);
	//global_->SetValue(cGLOUP, cROTATE, world_.rotate_);
	//global_->SetValue(cGLOUP, cNORMALPOSITION, vNormalPosition_);
}

void StageWall::SaveGlobalVariable()
{
	global_->AddItem(cGLOUP, cSIZE, vSize_);
	global_->AddItem(cGLOUP, cMODELSCALE, vModelScale_);
	global_->AddItem(cGLOUP, cMODIFIERSCALE, vModifierScale_);
	global_->AddItem(cGLOUP, cROTATE, world_.rotate_);
	global_->AddItem(cGLOUP, cNORMALPOSITION, vNormalPosition_);
}

void StageWall::LoadGlobalVariable()
{
	vSize_ = global_->GetFloatvalue(cGLOUP, cSIZE);
	vModelScale_ = global_->GetVector3value(cGLOUP, cMODELSCALE);
	vModifierScale_ = global_->GetVector3value(cGLOUP, cMODIFIERSCALE);
	world_.rotate_ = global_->GetVector3value(cGLOUP, cROTATE);
	vNormalPosition_ = global_->GetVector3value(cGLOUP, cNORMALPOSITION);
}

void StageWall::CalculateInit()
{
	world_.scale_.x = vModelScale_.x * vModifierScale_.x;
	world_.scale_.y = vModelScale_.y * vModifierScale_.y;
	world_.scale_.z = vModelScale_.z * vModifierScale_.z;

	// 五角形のどの位置に配置するか
	vNormalPosition_ = Normalize(vNormalPosition_);
	world_.translate_ = vSize_ * vNormalPosition_;
}

void StageWall::Initialize(size_t num)
{
	std::string tag = "wall";
	InstancingGameObject::Initialize(tag);

	global_ = GlobalVariables::GetInstance();

	// 名前登録
	tag = tag + std::to_string(num);
	//cMODELSCALE = tag + ":" + cMODELSCALE;
	//cMODIFIERSCALE = tag + ":" + cMODIFIERSCALE;
	cROTATE = tag + ":" + cROTATE;
	cNORMALPOSITION = tag + ":" + cNORMALPOSITION;

#ifdef _DEBUG

	// デバッグ時の生成エラー回避
	//SaveNewData();

	isLoadAllTime_ = true;

#endif // _DEBUG

	LoadGlobalVariable();

	collider_.reset(new SphereCollider);
	collider_->Initialize(tag, world_);

	world_.Initialize();

	CalculateInit();

	world_.UpdateMatrix();
}

void StageWall::Update()
{
	if (isLoadAllTime_)
	{
		LoadGlobalVariable();

		CalculateInit();
		global_->SetValue(cGLOUP, cNORMALPOSITION, vNormalPosition_);
		SaveGlobalVariable();
	}


	//行列更新
	world_.UpdateMatrix();
	//タグに対応したモデルにワールド追加
	IMM_->SetWorld(tag_, world_);
}

void StageWall::DebagWindow()
{
	// Stage.cpp 側から呼び出す
	//ImGui::Begin("StageWall");

	ImGui::Checkbox("LoadAllTime", &isLoadAllTime_);

	if (ImGui::Button("LoadConfig"))
	{
		LoadGlobalVariable();

		CalculateInit();
	}

	if (ImGui::Button("SaveConfig"))
	{
		SaveGlobalVariable();
	}
	if (ImGui::Button("SaveNewConfig"))
	{
		SaveNewData();
	}

	//ImGui::End();

	world_.UpdateMatrix();
}