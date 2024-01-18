#pragma once

#include "IGameObject/GameObject.h"
class Stage : public GameObject
{
private:
	enum ActionMode
	{
		None,   // 変更なし
		Shrink, // 縮める
		Expand, // 拡げる
	};

private:
	// ステージの最大値
	float kMaxRadius_ = 20.0f;

	// ステージの半径
	float radius_ = kMaxRadius_;

	// ステージに適用するスケールの倍率
	float kTransformRadius_ = 1.0f;

	ActionMode mode_ = None;

public:
	Stage();

	// 値の初期化
	void Initialize();

	void Update();

	void DebagWindow();

	void Draw(const Matrix4x4& viewp);

};
