#pragma once
#include <DxLib.h>

enum eObjectType
{
	NONE = 0,
	BLOCK,
	PLAYER,
	WALL,
	LIGHT,
	INVISIBLEFLOOR,
	PUSHBLOCK,
    ENEMY,
    REALENEMY,
	PLATE,
	OBJECTTYPE_COUNT // タイルの種類数（最後に追加）
	
};

struct TypeInfo
{
    eObjectType type;
    const char* name;
    int color;
};

// タイル情報テーブル（グローバルでもOK、StageEditorクラス内に static 配列でもOK）
static TypeInfo TYPE_INFO[OBJECTTYPE_COUNT] = {
    { NONE,          "None",      GetColor(55, 55, 55) },
    { BLOCK,         "Block",     GetColor(100, 200, 100) },
    { PLAYER,        "Player",    GetColor(200, 100, 100) },
    { WALL,          "Wall",      GetColor(150, 150, 150) },
    { LIGHT,         "Light",     GetColor(255, 255, 0) },
    { INVISIBLEFLOOR,"Invisible", GetColor(100, 100, 200) },
    { PUSHBLOCK,     "Push",      GetColor(200, 100, 200) },
    { ENEMY,         "Enemy",     GetColor(100, 100, 200) },
    { REALENEMY,     "REnemy",    GetColor( 50, 100, 200) }
    { PLATE,         "Plate",     GetColor(200, 0, 200)}
};

// ヘルパー関数
inline const TypeInfo& GetTypeInfo(eObjectType type)
{
    int idx = static_cast<int>(type);
    if (idx < 0 || idx >= OBJECTTYPE_COUNT)
        return TYPE_INFO[0]; // NONEを返す
    return TYPE_INFO[idx];
}
