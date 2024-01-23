#pragma once

#include"Vector3.h"
#include"WorldTransform/WorldTransform.h"

struct AABB {
	Vector3 minV;
	Vector3 maxV;
};

struct Sphere {
	Vector3 center;
	float radius;
};

//すべてのX回転量取得
float GetAllRotateX(const WorldTransform& world);
//すべてのY回転量取得
float GetAllRotateY(const WorldTransform& world);
//すべてのZ回転量取得
float GetAllRotateZ(const WorldTransform& world);
//親元も合わせた回転量取得
Vector3 GetAllRotate(const WorldTransform& world);


//AABBと円
bool InCollision(const AABB& a, const Sphere& s, Vector3& v);