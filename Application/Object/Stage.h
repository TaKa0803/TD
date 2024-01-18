#pragma once

#include "SingleGameObjects/GameObject.h"


class Stage : public GameObject
{
private:

private:
	// ステージの最大値
	float kMaxRadius_ = 20.0f;

	// ステージに適用するスケールの倍率
	float kTransformRadius_ = 1.0f;

public:
	Stage();

	// 値の初期化
	void Initialize();

	void Update();

	void DebagWindow();

	void Draw(const Matrix4x4& viewp);

};
