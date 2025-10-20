#include "ResourceManager.h"
#include "DxLib.h"
#include <stdexcept>

// �ÓI�����o�ϐ���`
ResourceManager* ResourceManager::instance = nullptr;

// �C���X�^���X�擾
ResourceManager* ResourceManager::GetInstance()
{
    if (!instance)
    {
        instance = new ResourceManager();
    }
    return instance;
}

// �C���X�^���X�폜
void ResourceManager::DeleteInstance()
{
    if (instance)
    {
        instance->UnloadResourcesAll();
        delete instance;
        instance = nullptr;
    }
}

// �摜�擾
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

// �����擾
const std::vector<int>& ResourceManager::GetSound(std::string file_name)
{
    if (sounds_container.count(file_name) == 0)
        CreateSoundResource(file_name);
    return sounds_container[file_name];
}

const std::vector<int>& ResourceManager::GetSound(const char* file_name)
{
    return GetSound(std::string(file_name));
}

const std::vector<int>& ResourceManager::GetSound(SoundParam element)
{
    return GetSound(element.file_path);
}

// ���ׂẴ��\�[�X���
void ResourceManager::UnloadResourcesAll()
{
    for (auto& pair : images_container)
    {
        for (auto h : pair.second) DeleteGraph(h);
        pair.second.clear();
    }

    for (auto& pair : sounds_container)
    {
        for (auto h : pair.second) DeleteSoundMem(h);
        pair.second.clear();
    }

    images_container.clear();
    sounds_container.clear();
}

void ResourceManager::CreateImagesResource(std::string file_name)
{
}

// �����摜�쐬
void ResourceManager::CreateImagesResource(std::string file_name, int all_num, int num_x, int num_y, int size_x, int size_y)
{
    int* handles = new int[all_num];
    int err = LoadDivGraph(file_name.c_str(), all_num, num_x, num_y, size_x, size_y, handles);
    if (err == -1) { delete[] handles; throw std::runtime_error(file_name + " ������܂���"); }

    for (int i = 0; i < all_num; i++) images_container[file_name].push_back(handles[i]);
    delete[] handles;
}

// �P�̂܂��͘A�ԉ摜�쐬
void ResourceManager::CreateImagesResourceSingle(std::string file_name, int all_num)
{
    std::vector<int> handles;
    if (all_num <= 1)
    {
        int h = LoadGraph(file_name.c_str());
        if (h == -1) throw std::runtime_error(file_name + " ������܂���");
        handles.push_back(h);
    }
    else
    {
        for (int i = 1; i <= all_num; ++i)
        {
            char buf[256];
            sprintf_s(buf, sizeof(buf), "%s%d.png", file_name.c_str(), i);
            int h = LoadGraph(buf);
            if (h == -1) throw std::runtime_error(std::string(buf) + " ������܂���");
            handles.push_back(h);
        }
    }
    images_container[file_name] = handles;
}

// �����쐬
void ResourceManager::CreateSoundResource(std::string file_name)
{
    int handle = LoadSoundMem(file_name.c_str());
    if (handle == -1) throw std::runtime_error(file_name + " ������܂���");
    sounds_container[file_name].push_back(handle);
}

