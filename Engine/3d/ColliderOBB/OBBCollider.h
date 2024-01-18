#pragma once
#include"InstancingGameObject/InstancingGameObject.h"

class OBBCollider : public InstancingGameObject {
public:

	void Initialize(const std::string& tag, const WorldTransform& parent);

	void Update();

	void Draw();

	void Debug(const char* name);

private:
	std::string colliderTag_;

	bool isDraw_ = true;

};