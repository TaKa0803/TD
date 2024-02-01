#pragma once

#include "SingleGameObjects/GameObject.h"
#include "SphereCollider/SphereCollider.h"

#include"Sprite/Sprite.h"

#include <optional>
#include <list>

class SomeEnemy;
class AreaAttack;

class BossEnemy :public GameObject
{
private:

	enum BEHAVIOR
	{
		IDOL,	// なにもしてない
		MOVE,	// 移動
		SUMMON,	// 雑魚敵召喚
		ATTACK,	// 範囲攻撃
		DAMAGE,	// 被弾
		CRUSH,	// 撃破
		SPECIAL,
		_COUNT,	// カウント用
	};

private:

	// 移動範囲
	// 中心からの距離
	float moveLength_ = 0.0f;
	
	// 召喚時間
	int32_t cSUMMONFRAME_ = 60;

	float cSIZE_ = 3.0f;

	// 移動にかかる時間
	int32_t cMOVEFRAME_ = 120;
	// 移動の回数とか期待値
	int moveCount_ = 0;

	// 攻撃にかかる時間
	int32_t cATTACKFRAME_ = 400;
	// 一度の攻撃回数
	int32_t cATTACKFREQUENCY_ = 6;
	// 攻撃が連続で出にくくなるやつ
	int attackCount_ = 0;

	// 次に向かう場所
	Vector2 nextPosition_ = { 0.0f,0.0f };

	// 今向いている方向
	Vector2 direction_ = { 0.0f, 0.0f };

	BEHAVIOR behavior_ = IDOL;
	std::optional<BEHAVIOR> reqBehavior_ = std::nullopt;

	std::unique_ptr<SphereCollider> collider_;

	// 雑魚敵
	std::list<std::unique_ptr<SomeEnemy>> enemies_;
	// 範囲攻撃
	std::list<std::unique_ptr<AreaAttack>> attacks_;

	uint32_t momentFrame_ = 20;

	bool isActive_ = false;

	uint32_t invisibleFrame_ = 60;
	bool isInvisible_ = false;

	// 被ダメージ
	float damage_ = 0.0f;

private:

#pragma region ボスのHPと表示
	//最大HP
	const int maxHP_ = 20;
	//現HP
	int HP_ = maxHP_;

	//ボスのHPバー
	std::unique_ptr<Sprite>hpBar_;
	std::unique_ptr<Sprite>hpBarBack_;
	std::unique_ptr<Sprite>hpBarFrame_;

	//画像へのパス
	std::string hpTex_ = "resources/AppResource/UI/BOSSGage.png";
	std::string hpBackTex_ = "resources/AppResource/UI/BOSSGage_Back.png";
	std::string hpFrameTex_ = "resources/AppResource/UI/BOSSGage_frame.png";

	//UI全体のワールド
	WorldTransform uiWorld_;
	//UI全体初期位置
	Vector3 uiPos_ = { 580,6,0 };
	Vector3 UIScale_ = { 0.6f,0.7f,1.0f };

	//HPバーの初期スケール
	Vector3 hpBarScale = { 962,56,1 };

	
#pragma endregion

#pragma region ボスの必殺技関係

	float goodGage_;

	float maxGoodGage_;

	enum SpecialATKState {
		wait,		//ため時間
		shotMove,	//攻撃発射モーション
		shotedWait,	//打った後の待機時間
		back,		//元の状態に戻る
		HitDown,	//あたって気絶時間
		kCountOfSpecialATKState	//数カウント
	};

	struct Count {
		int maxCount;
		int count;
	};

	struct SpecialATK
	{
		//特別攻撃ができる状態か
		bool isSpecialATK_ = false;

		//次のスペシャル攻撃が出るまでのカウント数
		const int maxNextSpecialATKCount = 60 * 3;

		//次の攻撃までのカウント
		int nextSpecialATKCount = 0;

		//状態のカウント
		int stateCount = 0;

		//各状態カウント
		Count count[kCountOfSpecialATKState];

		//攻撃を打ったか否か
		bool isShot = false;

		//弾の速度
		float ammoSpd_ = 0.2f;

		//攻撃弾のオブジェクトデータ
		std::unique_ptr<InstancingGameObject>ammo;

		//弾の速度
		Vector3 velocity_;

		//攻撃弾のコライダー
		std::unique_ptr<SphereCollider> collider;
	};

	SpecialATK specialATK{};


#pragma endregion



	//プレイヤーのworld情報
	const WorldTransform* playerW_;

public:
	BossEnemy();

	// 値の初期化
	void Initialize(const WorldTransform&player);

	void Update();

	void DebagWindow();

	void Draw(const Matrix4x4& viewp);

	SphereCollider* GetCollider() { return collider_.get(); }

	auto& GetEnemies() { return enemies_; }

	void OnCollision(float damage);

	bool GetIsActive() const { return isActive_; }

	//ゲージのポインタ取得
	void SetGage(float goodgage, float maxGood) { goodGage_ = goodgage; maxGoodGage_ = maxGood; }

	//攻撃弾の壁反射処理
	SphereCollider* GetSpecialATKCollider() { return specialATK.collider.get(); };

	void SPATKOnCollison(const Vector3&direc);
private:

	//プレイヤー方向を向く
	void SeePlayer();

	// 攻撃リストの更新
	void UpdateLists();

	void SummmonEnemy();
	// 範囲攻撃作成
	void CreateAttack();

	//HPバーの更新
	void HPBarUpdate();

	void UpdateIDOL();
	void UpdateMOVE();
	void UpdateSUMMON();
	void UpdateATTACK();
	void UpdateDAMAGE();
	void UpdateCRUSH();

	//必殺技の更新
	void UpdateSpecialATK();
};
