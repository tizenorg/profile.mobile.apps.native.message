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

#include "AttachmentHandler.h"
#include "Logger.h"
#include "MediaUtils.h"
#include "FileUtils.h"
#include "CallbackAssist.h"

using namespace Msg;

AttachmentHandler::AttachmentHandler(IAttachmentHandlerListener *listener, WorkingDirRef workingDir)
    : m_pListener(listener)
    , m_WorkingDir(workingDir)
    , m_Files()
    , m_CurrentFreeSpace(0)
    , m_CurrentFreeSlots(0)
    , m_IsResizing(false)
{
}

AttachmentHandler::~AttachmentHandler()
{
}

void AttachmentHandler::processFiles(const std::list <std::string> &files)
{
    TRACE;
    m_Files = files;
    ecore_thread_run(processRoutineCb, processEndCb, nullptr, this);
}

void AttachmentHandler::processRoutineCb(void *data, Ecore_Thread *tread)
{
    TRACE;
    AttachmentHandler* self = static_cast<AttachmentHandler*>(data);
    std::list <std::string>::iterator curFile;

    for(curFile = self->m_Files.begin(); curFile != self->m_Files.end(); curFile++)
    {
        MSG_LOG("Processing file: ", *curFile);
        if(!FileUtils::isExists(*curFile) || FileUtils::isDir(*curFile))
        {
            MSG_LOG_ERROR("File not exists: ", *curFile);
            self->m_IsResizing = false;
            break;
        }
        ECORE_SAFE_SYNC_CALL(AttachmentHandler, safeCallRequestStatus, self);

        if(self->m_CurrentFreeSlots < 1)
        {
            ECORE_SAFE_SYNC_CALL(AttachmentHandler, safeCallNoFreeSlots, self);
            self->m_IsResizing = false;
            break;
        }

        self->m_CurrentFile = self->m_WorkingDir->addFile(*curFile);
        if(FileUtils::getFileSize(self->m_CurrentFile) > self->m_CurrentFreeSpace)
        {
            if(!self->m_IsResizing)
            {
                self->m_IsResizing = true;
                ECORE_SAFE_SYNC_CALL(AttachmentHandler, safeCallNotifyResizeStart, self);
            }
            if(!MediaUtils::downgradeImageQuality(self->m_CurrentFile, self->m_CurrentFreeSpace))
            {
                self->m_WorkingDir->removeFile(self->m_CurrentFile);
                self->m_Files = std::list <std::string> (curFile, self->m_Files.end());
                ECORE_SAFE_SYNC_CALL(AttachmentHandler, safeCallNoFreeSpace, self);
                self->m_IsResizing = false;
                break;
            }
        }

        ECORE_SAFE_SYNC_CALL(AttachmentHandler, safeCallFileIsOk, self);
    }
}

void AttachmentHandler::processEndCb(void *data, Ecore_Thread *tread)
{
    TRACE;
    AttachmentHandler* self = static_cast<AttachmentHandler*>(data);
    ECORE_SAFE_SYNC_CALL(AttachmentHandler, safeCallNotifyResizeFinish, self);
}

void *AttachmentHandler::safeCallRequestStatus(void *data)
{
    TRACE
    AttachmentHandler* self = static_cast<AttachmentHandler*>(data);
    self->m_CurrentFreeSpace = self->m_pListener->OnAvailableSpaceRequest();
    self->m_CurrentFreeSlots = self->m_pListener->OnAvailableSlotsRequest();
    return nullptr;
}

void *AttachmentHandler::safeCallNoFreeSlots(void *data)
{
    TRACE
    AttachmentHandler* self = static_cast<AttachmentHandler*>(data);
    self->m_pListener->OnMaxAttachmentsReached();
    return nullptr;
}

void *AttachmentHandler::safeCallNoFreeSpace(void *data)
{
    TRACE
    AttachmentHandler* self = static_cast<AttachmentHandler*>(data);
    self->m_pListener->OnSpaceOverflow(self->m_Files);
    return nullptr;
}

void *AttachmentHandler::safeCallFileIsOk(void *data)
{
    TRACE
    AttachmentHandler* self = static_cast<AttachmentHandler*>(data);
    self->m_pListener->OnFileAttached(self->m_CurrentFile);
    return nullptr;
}

void *AttachmentHandler::safeCallNotifyResizeStart(void *data)
{
    AttachmentHandler* self = static_cast<AttachmentHandler*>(data);
    self->m_pListener->OnResizingStart();
    return nullptr;
}

void *AttachmentHandler::safeCallNotifyResizeFinish(void *data)
{
    TRACE
    AttachmentHandler* self = static_cast<AttachmentHandler*>(data);
    if(self->m_IsResizing)
        self->m_pListener->OnResizingDone();
    self->m_IsResizing = false;
    return nullptr;
}
