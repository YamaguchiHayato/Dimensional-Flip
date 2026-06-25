// Src/Parameter/ParameterTableBase.h
#pragma once
#include <unordered_map>

#include "Src/Utilty/TSVTableLoder.h"

namespace nsApp
{
    namespace nsSystem
    {
        struct ParameterRowReader
        {
            const TSVTable& table;
            int rowIndex;

            /**
             * @brief 指定された行のパラメータを読み取るためのコンストラクタ。
             * @param columnName 列名。
             * @param defaultValue デフォルト値。
             */
            float F(const char* columnName, float defaultValue) const
            {
                return table.GetFloat(rowIndex, columnName, defaultValue);
            }

            /**
             * @brief 指定された行のパラメータを読み取るためのコンストラクタ。
             * @param columnName 列名。
             * @param defaultValue デフォルト値。
             * @return 
             */
            int I(const char* columnName, int defaultValue) const
            {
                return table.GetInt(rowIndex, columnName, defaultValue);
            }
        };

        /**
         * @brief ParameterTable の共通処理（Load / Get / fallback）。
         * @tparam KeyType        キー型（std::string, WeaponType など）
         * @tparam ParameterType  パラメータ struct
         */
        template <typename KeyType, typename ParameterType> class ParameterTableBase
        {
        protected:
            /**
             * @brief パラメータテーブルを取得する。
             * @return パラメータテーブルへの参照
             */
            inline static std::unordered_map<KeyType, ParameterType>& GetTable()
            {
                static std::unordered_map<KeyType, ParameterType> table;
                return table;
            }


        public:
            template <typename KeyConverter, typename RowCreator>
            /**
             * @brief TSV ファイルからパラメータテーブルを読み込む。
             * @param[in] filePath       TSV ファイルのパス
             * @param[in] keyColumnName  キー列名
             * @param[in] keyConverter   キー変換関数（std::string → KeyType）
             * @param[in] rowCreator     行作成関数（TSV 行 → ParameterType）
             * @return 読み込み成功なら true
             */
            inline static bool Load(const char* filePath, const char* keyColumnName, KeyConverter keyConverter, RowCreator rowCreator)
            {
                return TSVTableLoder::LoadTable(filePath, keyColumnName, GetTable(), keyConverter, rowCreator);
            }

            /**
             * @brief キーに対応するパラメータを取得する。存在しない場合は fallback を返す。
             * @param key キー。
             * @return 対応するパラメータ、または fallback。
             */
            static const ParameterType& Get(const KeyType& key)
            {
                auto& table = GetTable();
                const auto it = table.find(key);
                if (it != table.end())
                    return it->second;

                static const ParameterType fallback;
                return fallback;
            }
        };
    }
} // namespace nsApp
