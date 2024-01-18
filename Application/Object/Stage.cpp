#include "Stage.h"
#include <imgui.h>

using Placement = StageWall::Placement;

Stage::Stage()
{
	for (size_t i = 0; i < Placement::_COUNT; i++)
	{
		walls_.emplace_back(new StageWall);
	}
}

void Stage::Initialize()
{
	for (size_t i = 0; i < Placement::_COUNT; i++)
	{
		walls_[i]->Initialize(i);
	}
}

void Stage::Update()
{
	for (size_t i = 0; i < Placement::_COUNT; i++)
	{
		walls_[i]->Update();
	}
}

void Stage::DebagWindow()
{
	ImGui::Begin("Stage");

	ImGui::Text("Walls:%d", walls_.size());
	ImGui::Separator();

	for (size_t i = 0; i < Placement::_COUNT; i++)
	{
		std::string tree = "wall:" + std::to_string(i);
		if (ImGui::TreeNode(tree.c_str()))
		{
			walls_[i]->DebagWindow();

			ImGui::TreePop();
		}
		ImGui::Separator();
	}

	if (ImGui::Button("SaveAllData"))
	{
		for (size_t i = 0; i < Placement::_COUNT; i++)
		{
			walls_[i]->SaveGlobalVariable();
		}
	}

	ImGui::End();


}