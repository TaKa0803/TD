#pragma once

#include "SingleGameObjects/GameObject.h"
#include "SphereCollider/SphereCollider.h"

#include"Sprite/Sprite.h"

#include <optional>
#include <list>

class SomeEnemy;

class BossEnemy :public GameObject
{
private:

	enum BEHAVIOR
	{
		IDOL,	// なにもしてない
		SUMMON,	// 雑魚敵召喚

		CRUSH,	// 撃破

		_COUNT,	// カウント用
	};

private:

	// 中心からの距離
	float moveLength_ = 0.0f;

	// 今向いている方向
	Vector2 direction_ = { 0.0f, 0.0f };

	BEHAVIOR behavior_ = IDOL;
	std::optional<BEHAVIOR> reqBehavior_ = std::nullopt;

	std::unique_ptr<SphereCollider> collider_;

	std::list<std::unique_ptr<SomeEnemy>> enemies_;

	uint32_t momentFrame_ = 20;

	bool isActive_ = false;

private:

#pragma region ボスのHPと表示
	//最大HP
	const int maxHP_ = 10;
	//現HP
	int HP_ = 10;

	//ボスのHPバー
	std::unique_ptr<Sprite>hpBar_;
	std::unique_ptr<Sprite>hpBarBack_;
	std::unique_ptr<Sprite>hpBarFrame_;

	//画像へのパス
	std::string hpTex_ = "resources/AppResource/UI/BOSSGage.png";
	std::string hpBackTex_ = "resources/AppResource/UI/BOSSGage_Back.png";
	std::string hpFrameTex_ = "resources/AppResource/UI/BOSSGage_frame.png";

	//HPバーの初期スケール
	Vector3 hpBarScale = { 962,56,1 };

#pragma endregion


public:
	BossEnemy();

	// 値の初期化
	void Initialize();

	void Update();

	void DebagWindow();

	void Draw(const Matrix4x4& viewp);

	SphereCollider* GetCollider() { return collider_.get(); }

	auto& GetEnemies() { return enemies_; }

	void OnCollision();

	bool GetIsActive() const { return isActive_; }

private:

	void SummmonEnemy();

	//HPバーの更新
	void HPBarUpdate();
};
