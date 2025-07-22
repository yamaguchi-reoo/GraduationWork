#include "ObjectManager.h"
#include <algorithm>
#include <DxLib.h>

#include "ObjectInfo.h"
#include "Character/Player/Player.h"
#include "Stage/Block.h"

#include <Windows.h>


ObjectManager::ObjectManager() {}

ObjectManager::~ObjectManager()
{
    Finalize(); 
}

void ObjectManager::Initialize()
{
	CreateObject<Player>(Vector2D(640.0f, 300.0f), Vector2D(50.0f, 50.0f)); // プレイヤーオブジェクトを生成
	CreateObject<Block>(Vector2D(640.0f, 360.0f), Vector2D(BLOCK_SIZE)); // プレイヤーオブジェクトを生成
}

void ObjectManager::Update()
{
    for (auto obj : objects)
    {
        if (obj) obj->Update();
    }

	GameObject* player = FindObjectType(eObjectType::PLAYER);

    if (player)
    {
        for (auto obj : objects)
        {
            if (obj && obj != player)
            {
                if (player->CheckBoxCollision(obj))
                {
                    player->OnHitCollision(obj);
                    obj->OnHitCollision(player);
                }
            }
        }
    }
}


void ObjectManager::Draw(Vector2D offset, double rate)
{
    for (auto obj : objects)
    {
        if (obj) obj->Draw(obj->GetLocation() - offset, rate);
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


