#include"IsCollisions.h"
#include <algorithm>

#pragma region 回転量取得関数
//すべてのX回転量取得
float GetAllRotateX(const WorldTransform& world) {
	if (world.parent_ != nullptr) {
		return world.rotate_.x + GetAllRotateX(*world.parent_);
	}
	else {
		return world.rotate_.x;
	}
}

//すべてのY回転量取得
float GetAllRotateY(const WorldTransform& world) {
	if (world.parent_ != nullptr) {
		return world.rotate_.y + GetAllRotateY(*world.parent_);
	}
	else {
		return world.rotate_.y;
	}
}

//すべてのZ回転量取得
float GetAllRotateZ(const WorldTransform& world) {
	if (world.parent_ != nullptr) {
		return world.rotate_.z + GetAllRotateZ(*world.parent_);
	}
	else {
		return world.rotate_.z;
	}
}

//親元も合わせた回転量取得
Vector3 GetAllRotate(const WorldTransform& world) {
	return{
		GetAllRotateX(world),
		GetAllRotateY(world),
		GetAllRotateZ(world)
	};
}
#pragma endregion

#pragma region Scale
float GetAllScaleX(const WorldTransform& world) {
	if (world.parent_ != nullptr) {
		return world.scale_.x * GetAllScaleX(*world.parent_);
	}
	else {
		return world.scale_.x;
	}
}

float GetAllScaleY(const WorldTransform& world) {
	if (world.parent_ != nullptr) {
		return world.scale_.y * GetAllScaleY(*world.parent_);
	}
	else {
		return world.scale_.y;
	}
}

float GetAllScaleZ(const WorldTransform& world) {
	if (world.parent_ != nullptr) {
		return world.scale_.z * GetAllScaleZ(*world.parent_);
	}
	else {
		return world.scale_.z;
	}
}

Vector3 GetAllScale(const WorldTransform& world) {
	return{
		GetAllScaleX(world),
		GetAllScaleY(world),
		GetAllScaleZ(world),
	};
}

#pragma endregion



//AABBと円
bool InCollision(const AABB& a, const Sphere& s, Vector3& v) {
	//最近接点を求める
	Vector3 closestPoint{ std::clamp(s.center.x,a.minV.x,a.maxV.x),
	std::clamp(s.center.y,a.minV.y,a.maxV.y),
	std::clamp(s.center.z,a.minV.z,a.maxV.z)
	};
	Sphere S{
		.center = closestPoint,
		.radius = 0.01f,
	};

	//
	Vector3 V = closestPoint - s.center;

	//最近接点と球の中心との距離を求める
 	float dis = Length(V);
	//距離が半径よりも小さければ衝突
	if (dis <= s.radius) { 
		//最近接点をわたす
		v = closestPoint;
		return true;
	}
	return false;
}