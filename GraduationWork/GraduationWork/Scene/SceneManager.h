#pragma once
#include "SceneBase.h"
#include "../Utility/FpsControl.h"
#include "../Utility/InputManager.h"

class SceneManager
{
private:
    SceneBase* current_scene;   //現在シーン情報のポインタ

	FpsControl fps_control;     //フレームレート制御クラス

public:
    SceneManager();
    ~SceneManager();


    void Initialize();  //初期化処理
    void Update();      //更新処理
    void Finalize();    //終了時処理
    void Draw();  //描画処理

private:
    //シーン切替処理
    void ChangeScene(eSceneType type);

    //シーン生成処理
    SceneBase* CreateScene(eSceneType type);

};


