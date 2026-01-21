#pragma once
#include "SceneBase.h"
#include "../Utility/FpsControl.h"
#include "../Utility/InputManager.h"
#include "../Utility/SoundManager.h"
#include "../Utility/ResourceManager.h"

class SceneManager
{
private:
    //現在シーン情報のポインタ
    SceneBase* current_scene;   

    //フレームレート制御クラス
	FpsControl fps_control;     


public:
    SceneManager();
    ~SceneManager();


    void Initialize();  //初期化処理
    void Update();      //更新処理
    void Finalize();    //終了時処理
    void Draw();  //描画処理
    static int font;

    static int titleFont;
    static int menuFont;


private:
    //シーン切替処理
    void ChangeScene(eSceneType category);

    //シーン生成処理
    SceneBase* CreateScene(eSceneType category);

};


