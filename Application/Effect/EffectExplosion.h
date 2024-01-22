#pragma once
#include"InstancingModelManager/InstancingModelManager.h"

#include"WorldTransform/WorldTransform.h"

#include<list>

class EffectExplosion {

public://シングルトンパターン
	static EffectExplosion* GetInstance();

private://シングルトンパターン

	EffectExplosion() = default;
	~EffectExplosion() = default;
	EffectExplosion(const EffectExplosion& o) = delete;
	const EffectExplosion& operator=(const EffectExplosion& o) = delete;

public:
	//各パーツのデータ
	struct moveData {
		//ワールドデータ
		WorldTransform world;
		//速度
		Vector3 velo;
		//加速度
		Vector3 acce;
		//各死亡カウント（フレーム数）
		int maxDeadCount;
		//実際のカウント（初期で0
		int deadCount = 0;
		//死亡フラグ
		bool isDead = false;
	};

	//エフェクトのデータ軍
	struct EffectData {
		//モデルのタグ
		std::string tag;

		//データ軍
		std::list<moveData*>mData;

		//データすべて死んだか否か
		bool isDeadData_ = false;
	};

	void Initialize();

	//エフェクトデータを追加
	void AddEffectData(const EffectData& eData);

	void Update();

	void Draw();

private:

	InstancingModelManager* IMM_;

	//データ
	std::list<EffectData*>datas_;

};