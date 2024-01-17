#pragma once
#include"Quaternion.h"
#include"IScene/IScene.h"


class MT4Scene : public IScene {
public:
	void Initialize() override;

	void Update() override;

	void Draw()override;


private:

	Quaternion ro1 = MakeRotateAxisAngleQuaternion({ 0.71f,0.71f,0.0f }, 0.3f);
	Quaternion ro2 = { -ro1.x,-ro1.y,-ro1.z,-ro1.w };

	Quaternion inter0 = Slerp(ro1, ro2, 0.0f);
	Quaternion inter1 = Slerp(ro1, ro2, 0.3f);
	Quaternion inter2 = Slerp(ro1, ro2, 0.5f);
	Quaternion inter3 = Slerp(ro1, ro2, 0.7f);
	Quaternion inter4 = Slerp(ro1, ro2, 1.0f);

};

