#pragma once
#include "InstancingGameObject/InstancingGameObject.h"
#include "GlobalVariables/GlobalVariables.h"
#include "ColliderOBB/OBBCollider.h"

class StageWall : public InstancingGameObject
{
public:

	enum Placement : uint32_t
	{
		Edge0 = 0,
		Edge1,
		Edge2,
		Edge3,
		Edge4,

		_COUNT,
	};


private:
	
	bool isLoadAllTime_ = false;

	GlobalVariables* global_ = nullptr;

	// 一つしかないもの
	std::string cGLOUP = "StageWall";
	std::string cSIZE = "Size";
	std::string cMODELSCALE = "ModelScale";
	std::string cMODIFIERSCALE = "ModifierScale";
	// 壁の数だけあるもの
	std::string cROTATE = "Rotate";
	std::string cNORMALPOSITION = "NormalPosition";

	// モデルの大きさ
	Vector3 vModelScale_ = { 1.0f,1.0f,1.0f };
	// ステージに適用するスケールの倍率
	Vector3 vModifierScale_ = { 1.0f,1.0f,1.0f };
	// ステージのどこに配置されるか
	// 基準 : 正規化された球
	Vector3 vNormalPosition_ = { 0.0f,0.0f,1.0f };
	// ステージ全体のサイズ
	float vSize_ = 1.0f;


	std::unique_ptr<OBBCollider> collider_;

private:

	void SaveNewData();
	void LoadGlobalVariable();

	// 値の初期化
	void CalculateInit();

public:
	// 値の初期化
	void Initialize(size_t num);

	void Update();

	void Draw();

	void DebagWindow();

	void SaveGlobalVariable();

	void OnCollision();

	OBBCollider* GetCollider() { return collider_.get(); }

	Vector3 GetDirection() const { return -vNormalPosition_; }
};
