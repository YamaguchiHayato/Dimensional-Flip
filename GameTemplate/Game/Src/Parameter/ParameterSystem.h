#pragma once

/** 
 * @file   ParameterSystem.h
 * @brief  パラメータシステムクラスの宣言
 * @detail 前作「エレメンタルハンターズ」から流用。
 */

#include <string>

namespace nsApp
{
    namespace nsSystem
    {
        class ParameterSystem
        {
        public:
            /* コンストラクタとデストラクタ。*/
            ParameterSystem() = default;
            virtual ~ParameterSystem() = default;


        public:
            /**
             * @brief 全てのパラメーターを読み込む。
             * @return　全てのパラメーターの読み込みに成功したか。
             */
            bool LoadAll();

            /**
             * @brief 全てのパラメーターテーブルを読み込む。
             * @return　全てのパラメーターテーブルの読み込みに成功したか。
             */
            bool LoadAllParameterTables();


        private:
            /**
             * @brief パラメーターのファイルパスを生成する。
             * @param fileName ファイル名
             * @return 生成されたファイルパス
             */
            std::string MakeParameterPath(const char* fileName) const;


        private:
            const std::string parameterDirectory_ = "Assets/Parameter/"; //! パラメーターファイルのディレクトリパス。

            bool isSuccess_ = true; //! パラメーター読み込みの成功フラグ。
        };

    }
}
