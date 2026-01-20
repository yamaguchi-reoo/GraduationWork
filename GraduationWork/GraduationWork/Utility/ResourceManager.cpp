#include "ResourceManager.h"
#include "DxLib.h"
#include <stdexcept>

// 静的メンバ変数定義
ResourceManager* ResourceManager::instance = nullptr;

// インスタンス取得
ResourceManager* ResourceManager::GetInstance()
{
    if (!instance)
    {
        instance = new ResourceManager();
    }
    return instance;
}

// インスタンス削除
void ResourceManager::DeleteInstance()
{
    if (instance)
    {
        instance->UnloadResourcesAll();
        delete instance;
        instance = nullptr;
    }
}

// 画像取得
const std::vector<int>& ResourceManager::GetImages(std::string file_name, int all_num, int num_x, int num_y, int size_x, int size_y)
{
    if (images_container.count(file_name) == 0)
    {
        if (num_x > 0 && num_y > 0)
            CreateImagesResource(file_name, all_num, num_x, num_y, size_x, size_y);
       else
            CreateImagesResourceSingle(file_name, all_num);
    }
    return images_container[file_name];
}

const std::vector<int>& ResourceManager::GetImages(const char* file_name, int all_num, int num_x, int num_y, int size_x, int size_y)
{
    return GetImages(std::string(file_name), all_num, num_x, num_y, size_x, size_y);
}

const std::vector<int>& ResourceManager::GetImages(MaterialParam element)
{
    return GetImages(element.file_path, element.all_num, element.num_x, element.num_y, element.size_x, element.size_y);
}

int ResourceManager::GetSE(const std::string& file)
{
    if (!se_container.count(file))
    {
		CreateSEResource(file);
    }
    return se_container[file];
}

int ResourceManager::GetBGM(const std::string& file)
{
    if(!bgm_container.count(file))
    {
        CreateBGMResource(file);
	}
    return bgm_container[file];
}

void ResourceManager::UnloadImage(const std::string& file)
{
    if (images_container.count(file))
    {
        for(auto h : images_container[file])DeleteGraph(h);
		images_container.erase(file);
    }
}

void ResourceManager::UnloadSE(const std::string& file)
{
    if (se_container.count(file))
    {
        DeleteSoundMem(se_container[file]);
        se_container.erase(file);
    }
}

void ResourceManager::UnloadBGM(const std::string& file)
{
    if (bgm_container.count(file))
    {
        DeleteSoundMem(bgm_container[file]);
        bgm_container.erase(file);
    }

}

// すべてのリソース解放
void ResourceManager::UnloadResourcesAll()
{
    for (auto& pair : images_container)
    {
        for (auto h : pair.second) DeleteGraph(h);
        pair.second.clear();
    }

    for(auto& pair : se_container)
    {
        DeleteSoundMem(pair.second);
	}

    for (auto& pair : bgm_container)
    {
        DeleteSoundMem(pair.second);
    }

    images_container.clear();
    se_container.clear();
	bgm_container.clear();
}

void ResourceManager::SetMasterVolume(int vol)
{
    SetVolumeSoundMem(vol, DX_PLAYTYPE_BACK);
}

void ResourceManager::SetSEVolume(int vol)
{
    se_volume = vol;
}

void ResourceManager::SetBGMVolume(int vol)
{
    bgm_volume = vol;
}

void ResourceManager::CreateImagesResource(std::string file_name)
{
}

// 分割画像作成
void ResourceManager::CreateImagesResource(std::string file_name, int all_num, int num_x, int num_y, int size_x, int size_y)
{
    int* handles = new int[all_num];
    int err = LoadDivGraph(file_name.c_str(), all_num, num_x, num_y, size_x, size_y, handles);
    if (err == -1) { delete[] handles; throw std::runtime_error(file_name + " がありません"); }

    for (int i = 0; i < all_num; i++) images_container[file_name].push_back(handles[i]);
    delete[] handles;
}

// 単体または連番画像作成
void ResourceManager::CreateImagesResourceSingle(std::string file_name, int all_num)
{
    std::vector<int> handles;
    if (all_num <= 1)
    {
        int h = LoadGraph(file_name.c_str());
        if (h == -1) throw std::runtime_error(file_name + " がありません");
        handles.push_back(h);
    }
    else
    {
        for (int i = 1; i <= all_num; ++i)
        {
            char buf[256];
            sprintf_s(buf, sizeof(buf), "%s%d.png", file_name.c_str(), i);
            int h = LoadGraph(buf);
            if (h == -1) throw std::runtime_error(std::string(buf) + " がありません");
            handles.push_back(h);
        }
    }
    images_container[file_name] = handles;
}

void ResourceManager::CreateSEResource(std::string file)
{
    int h = LoadSoundMem(file.c_str());
    if (h == -1) throw std::runtime_error(file + " がありません");
    ChangeVolumeSoundMem(se_volume, h);
    se_container[file] = h;
}

void ResourceManager::CreateBGMResource(std::string file)
{
    int h = LoadSoundMem(file.c_str()); // BGM も SoundMemで管理
    if (h == -1) throw std::runtime_error(file + " がありません");
    ChangeVolumeSoundMem(bgm_volume, h);
    bgm_container[file] = h;
}


