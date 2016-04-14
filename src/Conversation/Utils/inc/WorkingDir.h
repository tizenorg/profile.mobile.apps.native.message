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

#ifndef WorkingDir_h_
#define WorkingDir_h_

#include <string>
#include <memory>

namespace Msg
{
    class WorkingDir
    {
        public:
            WorkingDir();
            ~WorkingDir();

            WorkingDir(WorkingDir&) = delete;
            void operator=(WorkingDir&) = delete;

            bool isValid() const;
            const std::string &getPath() const;

            std::string genUniqueFilePath(const std::string &fileName) const;
            std::string addFile(const std::string &path);
            std::string addTextFile(const std::string &text, const std::string &fileName = "");
            void removeFile(const std::string &path);
            void clear();

        private:
            void createWorkingDir();
            void removeWorkingDir();
            std::string getUniqueDirName(const std::string &path);

        private:
            std::string m_Path;
    };

    typedef std::shared_ptr<WorkingDir> WorkingDirRef;
}

#endif /* WorkingDir_h_ */
