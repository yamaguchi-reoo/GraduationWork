#include "ObjectManager.h"
#include <algorithm>
#include <DxLib.h>

#include "ObjectInfo.h"
#include "ObjectList.h"

#include <Windows.h>


ObjectManager::ObjectManager() {}

ObjectManager::~ObjectManager()
{
    Finalize(); 
}

void ObjectManager::Initialize()
{
	//CreateObject<Player>(Vector2D(640.0f, 300.0f), Vector2D(50.0f, 50.0f)); // プレイヤーオブジェクトを生成
	//CreateObject<Block>(Vector2D(640.0f, 360.0f), Vector2D(BLOCK_SIZE)); // プレイヤーオブジェクトを生成
}

void ObjectManager::Update()
{
    for (auto obj : objects)
    {
        if (obj) obj->Update();
    }

	GameObject* player_f = FindObjectType(eObjectType::PLAYER);

    if (player_f)
    {
        for (auto obj : objects)
        {
            if (obj && obj != player_f)
            {
                if (player_f->CheckBoxCollision(obj))
                {
                    player_f->OnHitCollision(obj);
                    obj->OnHitCollision(player_f);
                }
            }
        }
    }
}

void ObjectManager::Draw(Vector2D offset, double rate)
{
    for (auto obj : objects)
    {
        if (obj)
        {
            Vector2D draw_location = obj->GetLocation() - offset;
            obj->Draw(draw_location, rate);
        }
    }
}

void ObjectManager::Finalize()
{
    //オブジェクトが空の場合は処理をスキップ
    if (objects.empty())
    {
        return;
    }
    for (auto obj : objects)
    {
        if (obj)
        {
            obj->Finalize();
            delete obj;
        }
    }
    objects.clear();
}

void ObjectManager::DeleteObject(GameObject* obj)
{
    if (obj == nullptr) return;

    auto it = std::find(objects.begin(), objects.end(), obj);

    if (it != objects.end()) {
        //見つかった場合、オブジェクトを削除
        (*it)->Finalize(); //終了処理
        delete* it;         //メモリ解放
        objects.erase(it);  //リストから削除
    }
}

GameObject* ObjectManager::FindObjectType(eObjectType type)
{
    for (auto obj : objects)
    {
        if (obj && obj->GetObjectType() == type)
        {
            return obj;
        }
    }
    return nullptr;
}


