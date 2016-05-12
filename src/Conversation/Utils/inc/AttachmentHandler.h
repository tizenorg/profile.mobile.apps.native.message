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

#ifndef AttachmentHandler_h_
#define AttachmentHandler_h_

#include <list>
#include <string>
#include <Ecore.h>
#include "WorkingDir.h"

namespace Msg
{
    class IAttachmentHandlerListener;

    class AttachmentHandler
    {
        public:
            AttachmentHandler(WorkingDirRef workingDir);
            virtual ~AttachmentHandler();

            /**
             * Run checking and resizing of files asynchronously.
             * Invokes functions of IAttachmentHandlerListener.
             */
            void processFile(const std::string &file, long long freeSpace);
            void setListener(IAttachmentHandlerListener *listener);
        private:
            static void processRoutineCb(void *data, Ecore_Thread *tread);

            static void *safeCallOnFileReady(void *data);
            static void *safeCallOnFileFails(void *data);

            void onRoutine();
        private:
            IAttachmentHandlerListener *m_pListener;
            WorkingDirRef m_WorkingDir;
            std::string m_CurrentFile;
            long long m_CurrentFreeSpace;
    };

    class IAttachmentHandlerListener
    {
        public:
            virtual ~IAttachmentHandlerListener() {};
            virtual void onFileReady(const std::string &filePath) = 0;
            virtual void onFileFails() = 0;
    };
}

#endif /* AttachmentHandler_h_ */
