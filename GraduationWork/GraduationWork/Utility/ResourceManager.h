#pragma once
#include <map>
#include <string>
#include <vector>

// 画像用パラメータ
struct MaterialParam
{
    std::string file_path;      // ファイルパス
    int all_num;                // 総画像数
    int num_x;                  // 横分割数（均等スプライト用）
    int num_y;                  // 縦分割数（均等スプライト用）
    int size_x;                 // 各フレーム幅
    int size_y;                 // 各フレーム高さ
    std::vector<int> row_counts; // 行ごとの横列数（不規則スプライト用）
};

// 音源用パラメータ
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

    // 画像取得
    const std::vector<int>& GetImages(std::string file_name, int all_num = 1, int num_x = 1, int num_y = 1, int size_x = 0, int size_y = 0);
    const std::vector<int>& GetImages(const char* file_name, int all_num = 1, int num_x = 1, int num_y = 1, int size_x = 0, int size_y = 0);
    const std::vector<int>& GetImages(MaterialParam element);

    // 音源取得
    const std::vector<int>& GetSound(std::string file_name);
    const std::vector<int>& GetSound(const char* file_name);
    const std::vector<int>& GetSound(SoundParam element);

    void UnloadResourcesAll();

private:
    // 画像リソース作成
    void CreateImagesResource(std::string file_name);
    void CreateImagesResource(std::string file_name, int all_num, int num_x, int num_y, int size_x, int size_y);
    void CreateImagesResourceSingle(std::string file_name, int all_num);

    // 音源リソース作成
    void CreateSoundResource(std::string file_name);

};
