#pragma once
#include"Instancing/InstancingGameObject.h"
#include"SphereCollider/SphereCollider.h"
#include<iostream>
#include"item.h"


class Enemy : public InstancingGameObject {

public:
	/// <summary>
	/// 
	/// </summary>
	void Initialize(const Vector3& position, const WorldTransform* playerWorld);

	/// <summary>
	/// 
	/// </summary>
	void Update();

	void Collision(const SphereCollider&collider);

	/// <summary>
	/// 
	/// </summary>
	void Draw();

	//死んだか否か
	bool GetDead() { return isDead_; }

private:
	//プレイヤーのワールド
	const WorldTransform* playerWorld_;

	std::unique_ptr<SphereCollider>collider_;

#pragma region モデル関係
	//モデルの数
	static const int modelNum_ = 5;

	//タグ軍
	std::string tags[modelNum_] = {
		"Head",
		"LArm",
		"RArm",
		"LFoot",
		"RFoot"
	};

	enum Parts {
		HEAD,
		LARM,
		RARM,
		LFOOT,
		RFOOT
	};

	WorldTransform mWorlds[modelNum_] = {};


	//歩くアニメーション
	ModelAnimeParts walkData_;

	//止まったモーション
	ModelAnimeParts stopData_;

	ModelAnimeParts nowRoop_;

	int roopCount_ = 0;

	enum MoveState {
		StopS,
		MoveS,
		NoneS
	};

	MoveState moveState_ = NoneS;
#pragma endregion



	//移動速度
	Vector3 velocity_{};

	//最大速度になるまでのF数
	float maxSPDFrame = 60.0f;

	//個体差用
	const float maxSPD_ = 0.5f;
	const float minSPD_ = 0.3f;

	//移動速度
	float moveSPD_ = 0.5f;


	//ugokanakunarukyori
	float stopRange_ =10.0f;

	//探知距離
	float serchRange_ = 30.0f;


	enum State {
		Normal,
		Hit
	};

	State state_ = Normal;


	const Vector3 hitVelo = { 0,1.5f,0 };
	//加速度
	Vector3 acce = { 0,-0.1f,0 };

	//吹っ飛び量
	float hitSPD_ = 0.5f;


	bool isDead_=false;

	int HP_ = 1;

	float tHeight = 0;
};