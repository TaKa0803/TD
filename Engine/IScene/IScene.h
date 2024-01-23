#pragma once


//基底クラス
class IScene {
protected:
	//シーン番号
	static int sceneNo;

	static bool leaveGame;
public:

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	virtual ~IScene();

	/// <summary>
	/// シーン番号取得
	/// </summary>
	/// <returns>シーン番号</returns>
	int GetSceneNo();

	//シーンセット
	void SetSceneNo(int scene);

	//ゲーム終了フラグがONになっているか
	static bool GetLeaveScene() { return leaveGame; };

};