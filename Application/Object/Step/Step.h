#pragma once
#include"InstancingGameObject/InstancingGameObject.h"

class Step : public InstancingGameObject {

public:

	Step();

	void Initialize();

	void Update();

	void Draw();

	void Debug();

public:

	void SetTranslate(const Vector3& translate) { world_.translate_ = translate; }

	void SetRotate(float y) { world_.rotate_.y = y; }
};
