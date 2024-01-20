#pragma once

#include"InstancingModelManager/InstancingModelManager.h"
#include"WorldTransform/WorldTransform.h"
#include<vector>

class InstancingGameObject {

protected:
	//モデルのタグ
	std::string tag_;

	//ワールド
	WorldTransform world_{};

	//インスタンシングモデルマネージャー
	InstancingModelManager* IMM_=nullptr;

public:
	//ワールド取得
	const WorldTransform& GetWorld()const  { return world_; }

	//開放処理
	virtual ~InstancingGameObject();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="tag">モデルタグ</param>
	virtual void Initialize(const std::string& tag);

	/// <summary>
	/// 行列とワールド追加
	/// </summary>
	virtual void Update();

};

