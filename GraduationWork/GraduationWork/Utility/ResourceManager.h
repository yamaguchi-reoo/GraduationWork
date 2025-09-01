#pragma once
#include <map>
#include <string>
#include <vector>

// �摜�p�p�����[�^
struct MaterialParam
{
    std::string file_path;      // �t�@�C���p�X
    int all_num;                // ���摜��
    int num_x;                  // ���������i�ϓ��X�v���C�g�p�j
    int num_y;                  // �c�������i�ϓ��X�v���C�g�p�j
    int size_x;                 // �e�t���[����
    int size_y;                 // �e�t���[������
    std::vector<int> row_counts; // �s���Ƃ̉��񐔁i�s�K���X�v���C�g�p�j
};

// �����p�p�����[�^
struct SoundParam
{
    std::string file_path;
};

class ResourceManager
{
private:
    static ResourceManager* instance;

    std::map<std::string, std::vector<int>> images_container;
    std::map<std::string, std::vector<int>> sounds_container;

private:
    ResourceManager() = default;
    ResourceManager(ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

public:
    ~ResourceManager() = default;

    static ResourceManager* GetInstance();
    static void DeleteInstance();

    // �摜�擾
    const std::vector<int>& GetImages(std::string file_name, int all_num = 1, int num_x = 1, int num_y = 1, int size_x = 0, int size_y = 0);
    const std::vector<int>& GetImages(const char* file_name, int all_num = 1, int num_x = 1, int num_y = 1, int size_x = 0, int size_y = 0);
    const std::vector<int>& GetImages(MaterialParam element);

    // �����擾
    const std::vector<int>& GetSound(std::string file_name);
    const std::vector<int>& GetSound(const char* file_name);
    const std::vector<int>& GetSound(SoundParam element);

    void UnloadResourcesAll();

private:
    // �摜���\�[�X�쐬
    void CreateImagesResource(std::string file_name);
    void CreateImagesResource(std::string file_name, int all_num, int num_x, int num_y, int size_x, int size_y);
    void CreateImagesResourceSingle(std::string file_name, int all_num);

    // �������\�[�X�쐬
    void CreateSoundResource(std::string file_name);

};
