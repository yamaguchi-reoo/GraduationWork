#pragma once
#include <vector>
#include "../Object/GameObject.h"

enum class eSceneType
{
	TITLE,
	GAME_MAIN,
};

class SceneBase
{
protected:
	std::vector<GameObject*>objects;
	Vector2D camera_location;
public:
	//コンストラクタ
	SceneBase();
	//デストラクタ
	virtual ~SceneBase();

	virtual void Initialize();		//初期化処理
	virtual eSceneType Update();	//更新処理
	virtual void Draw();		//描画処理
	virtual void Finalize();		//終了時処理

	//現在のシーンタイプを取得する
	virtual eSceneType GetNowSceneType()const = 0;

	void DeleteObject(GameObject* obj);

protected:
	//ゲームオブジェクトを生成
	template <class T>
	T* CreateObject(const Vector2D& _location, const Vector2D& _box_size);
};


