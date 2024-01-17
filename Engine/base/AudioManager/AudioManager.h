#pragma once
#include<string>
#include<map>
#include<wrl.h>

#include"AudioManager/SoundData.h"





//音声データ
struct SoundData {
	//波形フォーマット
	WAVEFORMATEX wfex;
	//バッファの先頭アドレス
	BYTE* pBuffer;
	//バッファのサイズ
	unsigned int bufferSize;
};


class AudioManager {

public://シングルトンパターン
	static AudioManager* GetInstance();

private://シングルトンパターン

	AudioManager() = default;
	~AudioManager() = default;
	AudioManager(const AudioManager& o) = delete;
	const AudioManager& operator=(const AudioManager& o) = delete;


public:

	//初期化
	void Initialize();

	void Finalize();

	//全モデルの描画
	void LoadAllSoundData();

	//音の再生
	void PlaySoundData(const int dataNum);

private:

	SoundData LoadSoundData(const char* name);

private:

	//音楽の音声ファイル
	const std::string fileName_ = "resources/jsonfile/resourcePath.json";

	//ファイルのグループ名
	const std::string groupName_ = "audios";

	//サウンドフォルダまでのパス
	const std::string foldaPath ="resources/sounds/";

	Microsoft::WRL::ComPtr<IXAudio2> xAudio2=nullptr;

	IXAudio2MasteringVoice* masterVoice=nullptr;


	

	//音のデータの数
	int soundNum_ = 0;

	//要素の番号とタグ名の塊
	std::map<std::string,int>tagDatas_;
	//タグ名とデータの塊
	std::map<int, SoundData*>soundDatas_;
};