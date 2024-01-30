#pragma once

#include "InstancingGameObject/InstancingGameObject.h"
#include "SphereCollider/SphereCollider.h"

#include <optional>

class SomeEnemy :public InstancingGameObject
{
public:

	enum BEHAVIOR
	{
		IDOL,	// なにもしてない
		MOVE,	// 移動
		BURST,	// 弾かれ中
		DESTROY,// 爆発

		_COUNT,	// カウント用
	};

private:

	int32_t cBURSTFRAME_ = 300;
	int32_t cALIVEFRAME_ = 600;

	// 中心からの距離
	float moveLength_ = 0.0f;

	// 今向いている方向
	Vector3 direct3_ = { 0.0f,0.0f, 0.0f };

	BEHAVIOR behavior_ = IDOL;
	std::optional<BEHAVIOR> reqBehavior_ = std::nullopt;

	std::unique_ptr<SphereCollider> collider_;

	// 爆発までのフレーム
	uint32_t momentFrame_ = cBURSTFRAME_;

	bool isActive_ = false;

	// 既にボスに当たっているか
	bool isHit_ = false;

public:
	// 値の初期化
	void Initialize();

	void Update();

	void DebagWindow(int num);

	void Draw();

	SphereCollider* GetCollider() { return collider_.get(); }

	// ボスとぶつかった時
	bool OnCollision()
	{
		if (isHit_) { return false; }
		reqBehavior_ = DESTROY;
		isHit_ = true;
		return true;
	}
	// 壁、攻撃の時
	void OnCollision(const Vector3& direction);

	bool GetIsActive() const { return isActive_; }
	bool GetIsBurst() const { return behavior_ == BURST; }
	bool GetIsDestroy() const { return behavior_ == DESTROY; }

	void SetBehavior(BEHAVIOR b) { reqBehavior_ = b; }
	void SetPosition(const Vector3& pos) { world_.translate_ = pos; }
private:
};
