#include "MT4Scene.h"
#include<imgui.h>

void DrawQuaternion(const char* name,const Quaternion&q) {

	ImGui::Begin(name);
	ImGui::Text("%4.2f,%4.2f,%4.2f,%4.2f",q.x,q.y,q.z,q.w);
	ImGui::End();
}



void MT4Scene::Initialize() {

}

void MT4Scene::Update() {

	

	DrawQuaternion("inter0", inter0);
	DrawQuaternion("inter1", inter1);
	DrawQuaternion("inter2", inter2);
	DrawQuaternion("inter3", inter3);
	DrawQuaternion("inter4", inter4);

}

void MT4Scene::Draw() {

}


