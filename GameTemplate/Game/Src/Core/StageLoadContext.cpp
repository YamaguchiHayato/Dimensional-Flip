#include "stdafx.h"

#include <fstream>

#include "Src/Core/StageLoadContext.h"
#include "Src/Parameter/Stage/StageMasterTable.h"

namespace
{
    /**
     * @brief バイナリファイルを丸ごと読み込む。
     * @param path ファイルパス。
     * @return 読み込みデータ。失敗時は空。
     */
    std::vector<char> ReadFileBinary(const std::string& path)
    {
        std::ifstream ifs(path, std::ios::binary | std::ios::ate);
        if (!ifs)
            return {};

        const std::streamsize size = ifs.tellg();
        if (size <= 0)
            return {};

        std::vector<char> buffer(static_cast<size_t>(size));
        ifs.seekg(0, std::ios::beg);
        ifs.read(buffer.data(), size);

        if (!ifs)
            return {};

        return buffer;
    }
} // namespace

namespace nsApp
{
    namespace nsStage
    {
        std::mutex StageLoadContext::mutex_;
        std::vector<char> StageLoadContext::modelBuffer_;
        std::string StageLoadContext::modelPath_;
        bool StageLoadContext::isReady_ = false;

        void StageLoadContext::Reset()
        {
            std::lock_guard<std::mutex> lock(mutex_);
            modelBuffer_.clear();
            modelPath_.clear();
            isReady_ = false;
        }

        void StageLoadContext::PrepareOnWorker(StageID stageID)
        {
            // TSV は起動時に読み込み済み。Get は読み取り専用なのでワーカーから呼んで OK。
            const auto& master = nsSystem::StageMasterTable::Get(stageID);

            std::vector<char> buffer;
            std::string path;

            if (master.stageID != StageID::sInvalid && !master.modelPath.empty())
            {
                path = master.modelPath;
                buffer = ReadFileBinary(path);
            }

            std::lock_guard<std::mutex> lock(mutex_);
            modelPath_ = std::move(path);
            modelBuffer_ = std::move(buffer);
            isReady_ = !modelBuffer_.empty();
        }

        bool StageLoadContext::IsReady()
        {
            std::lock_guard<std::mutex> lock(mutex_);
            return isReady_;
        }

        std::string StageLoadContext::GetModelPath()
        {
            std::lock_guard<std::mutex> lock(mutex_);
            return modelPath_;
        }

        const std::vector<char>& StageLoadContext::GetModelBuffer()
        {
            // J-3 で Main Job 完了後にのみ呼ぶ想定。
            return modelBuffer_;
        }
    } // namespace nsStage
} // namespace nsApp
