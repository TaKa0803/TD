#pragma once
#include"SingleGameObjects/GameObject.h"

class Plane :public GameObject {

public:

	Plane();

	void Initialize();

	void DebagWindow();

	void Draw(const Matrix4x4& viewp);


};

