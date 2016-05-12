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
            AttachmentHandler(IAttachmentHandlerListener *listener, WorkingDirRef workingDir);
            virtual ~AttachmentHandler();

            /**
             * Run checking and resizing of files asynchronously.
             * Invokes functions of IAttachmentHandlerListener.
             */
            void processFiles(const std::list <std::string> &files);
        private:
            static void processRoutineCb(void *data, Ecore_Thread *tread);
            static void processEndCb(void *data, Ecore_Thread *tread);

            static void *safeCallRequestStatus(void *data);
            static void *safeCallNoFreeSlots(void *data);
            static void *safeCallNoFreeSpace(void *data);
            static void *safeCallFileIsOk(void *data);
            static void *safeCallNotifyResizeStart(void *data);
            static void *safeCallNotifyResizeFinish(void *data);
        private:
            IAttachmentHandlerListener *m_pListener;
            WorkingDirRef m_WorkingDir;
            std::list <std::string> m_Files;
            std::string m_CurrentFile;
            long long m_CurrentFreeSpace;
            int m_CurrentFreeSlots;
            bool m_IsResizing;
    };

    class IAttachmentHandlerListener
    {
        public:
            virtual ~IAttachmentHandlerListener() {};
            virtual int OnAvailableSlotsRequest() = 0;
            virtual long long OnAvailableSpaceRequest() = 0;
            virtual void OnResizingStart() = 0;
            virtual void OnResizingDone() = 0;
            virtual void OnFileAttached(const std::string &filePath) = 0;
            virtual void OnSpaceOverflow(const std::list <std::string> &tooBigFiles) = 0;
            virtual void OnMaxAttachmentsReached() = 0;
    };
}

#endif /* AttachmentHandler_h_ */
