/*
 * Copyright (c) 2009-2015 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef __MSG_FILE_UTIL_H__
#define __MSG_FILE_UTIL_H__

#include <string>

namespace Msg
{
    class FileUtils
    {
        public:
            /**
             * @brief Get file size.
             * @param[in] path to file
             * @retval file size
             */
            static long long getFileSize(const std::string &file);

            /**
             * @brief Check file existing.
             * @param[in] path to file
             * @retval Returns true if file exist, else false
             */
            static bool isExists(const std::string &file);

            /**
             * @brief Copy file.
             * @param[in] src Source
             * @param[in] dst dst
             * @retval Returns true if file exist, else false
             */
            static bool copy(const std::string &src, const std::string &dst);

            /**
             * @brief Make Directory.
             * @param[in] path to directory
             * @retval Returns true if directory created, else false
             */
            static bool makeDir(const std::string &dir);

            /**
             * @brief Check directory.
             * @param[in] file File Directory
             * @retval Returns true if file is directory, else false
             */
            static bool isDir(const std::string &file);

            /**
             * @brief Strip file extension.
             * @param[in] path File Path
             * @retval Returns file without extension.
             */
            static std::string stripExtension(const std::string &path);

            /**
             * @brief Split file path (base-path + file-name + file-extension).
             * @param[in] file path name
             * @param[out] base path(with '/' at end)
             * @param[out] file name
             * @param[out] extension(without dot)
             */
            static void splitPath(const std::string &path, std::string &basePath,
                                 std::string &fileName, std::string &extension);

            /**
             * @brief Get file name with extension
             * @param[in] file path name
             * @retval Returns file name with extension
             */
            static std::string getFileName(const std::string &path);

            /**
             * @brief Remove recursive files and directorys.
             * @param[in] path File or directory
             * @param[in] true - remove current dir, false - otherwise
             * @retval Returns true if all success, false otherwise
             */
            static bool remove(const std::string &path, bool removeCurrentDir = true);

            /**
             * @brief Read text file
             * @param[in] file path name
             * @retval Returns text content
             */
            static std::string readTextFile(const std::string &path);

            /**
             * @brief Write text file
             * @param[in] file path name
             * @param[in] text content
             * @return Returns true if all success, false otherwise
             */
            static bool writeTextFile(const std::string &path, const std::string &text);

            /**
             * @brief Generate new file path
             * @param[in] storage path
             * @param[in] file path name
             * @return Returns unique file path in storage directory
             */
            static std::string genUniqueFilePath(const std::string &storagePath, const std::string &filePath);
    };
}

#endif /* __MSG_FILE_UTIL_H__ */
