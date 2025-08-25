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

void ObjectManager::Update(Vector2D offset)
{
    for (auto obj : objects)
    {
        if (IsOnScreen(obj, offset))
        {
            obj->Update();
        }
    }

     // 全オブジェクト同士の衝突判定（重複チェック回避のため片側ループ）
    for (size_t i = 0; i < objects.size(); ++i)
    {
        for (size_t j = i + 1; j < objects.size(); ++j)
        {
            auto obj_a = objects[i];
            auto obj_b = objects[j];
            if (obj_a && obj_b)
            {
                if (obj_a->CheckBoxCollision(obj_b))
                {
                    obj_a->OnHitCollision(obj_b);
                    obj_b->OnHitCollision(obj_a);
                }
            }
        }
    }

    GameObject* enemy_f = FindObjectType(eObjectType::ENEMY);

    if (enemy_f)
    {
        for (auto obj : objects)
        {
            if (obj && obj != enemy_f)
            {
                if (enemy_f->CheckBoxCollision(obj))
                {
                    enemy_f->OnHitCollision(obj);
                    obj->OnHitCollision(enemy_f);
                }
            }
        }
    }




    //Update後に削除を実行
    for (auto obj : delete_objects)
    {
        DeleteObject(obj);
    }
    delete_objects.clear();
}

void ObjectManager::Draw(Vector2D offset, double rate)
{
    for (auto obj : objects)
    {
        if (IsOnScreen(obj, offset))
        {
            obj->Draw(offset, rate);
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

void ObjectManager::RequestDeleteObject(GameObject* obj)
{
    if (obj && std::find(delete_objects.begin(), delete_objects.end(), obj) == delete_objects.end()) {
        delete_objects.push_back(obj);
    }
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

std::vector<GameObject*> ObjectManager::GetObjects(eObjectType type)
{
    std::vector<GameObject*> result;

    for (auto* obj : objects)
    {
        if (obj->GetObjectType() == type)
        {
            result.push_back(obj);
        }
    }

    return result;
}

bool ObjectManager::IsOnScreen(GameObject* obj, Vector2D offset)
{
    if (!obj) return false;

    Vector2D loc = obj->GetLocation();
    Vector2D size = obj->GetBoxSize();

    float screen_left = offset.x;
    float screen_right = offset.x + SCREEN_WIDTH;
    float screen_top = offset.y;
    float screen_bottom = offset.y + SCREEN_HEIGHT;

    return loc.x + size.x > screen_left &&
        loc.x < screen_right &&
        loc.y + size.y > screen_top &&
        loc.y < screen_bottom;
}


