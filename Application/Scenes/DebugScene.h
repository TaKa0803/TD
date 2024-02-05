#pragma once

#include "IScene/IScene.h"
#include"Input/Input.h"
#include"Camera/Camera.h"
#include<iostream>
#include"SphereCollider/SphereCollider.h"
#include"ColliderPlane/PlaneCollider.h"
#include"ColliderOBB/OBBCollider.h"

#include"Effect/EffectExplosion.h"

class DebugScene : public IScene {

public:

	DebugScene();

	~DebugScene();

	void Initialize() override;

	void Update() override;

	void Draw() override;

private:

	void Debug();

private:
	//キー入力
	Input* input_ = nullptr;

	WorldTransform cWorld_;
	std::unique_ptr<Camera> camera_;

	/*std::unique_ptr<InstancingGameObject>pHead_;
	std::unique_ptr<InstancingGameObject>pLA_;
	std::unique_ptr<InstancingGameObject>pRA_;
	std::unique_ptr<InstancingGameObject>pLF_;
	std::unique_ptr<InstancingGameObject>pRF_;
	std::unique_ptr<InstancingGameObject>pHI_;*/

#pragma region MyRegion


	std::unique_ptr<InstancingGameObject>h_;
	std::unique_ptr<InstancingGameObject>B_;
	std::unique_ptr<InstancingGameObject>LA_;
	std::unique_ptr<InstancingGameObject>RA_;


	enum PARTS {
		HEAD,
		BODY,
		LARM,
		RARM,
		_count,
	};

	enum ANIMETYPE {
		NORMAL,
		GOOD,
		BAD,
		_countOfAnimeType
	};

	struct StEd {
		Vector3 st{};
		Vector3 ed{};
	};

	struct Parts {
		StEd pos;
		StEd rotate;
	};

	struct AnimeData {
		//パーツデータ
		Parts parts[_count];

		Vector3 pos[_count];
		Vector3 rotate[_count];


		//最大フレーム数
		int maxCount = 60;

		//フレームカウント
		int count = 0;

		//ループするか
		bool isLoop = true;
	};

	AnimeData animeData[_countOfAnimeType];

	AnimeData nowAnime;

	ANIMETYPE animeType = NORMAL;

	void Animation();
#pragma endregion

	//エフェクト
	EffectExplosion* EffectExp_;

	//エフェクト
	std::string eTag_ = "box";
};