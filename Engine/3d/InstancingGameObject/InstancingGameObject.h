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

	//色
	Vector4 color = { 1,1,1,1 };
public:
	//ワールド取得
	const WorldTransform& GetWorld()const  { return world_; }

	void SetTranslate(const Vector3& trans) { world_.translate_ = trans; }

	void SetScale(float scale) { world_.scale_ = { scale,scale ,scale }; }

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

	virtual void Draw();
};

