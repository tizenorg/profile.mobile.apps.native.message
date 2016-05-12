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
#include "MediaType.h"
#include "FileUtils.h"
#include "CallbackAssist.h"

using namespace Msg;

AttachmentHandler::AttachmentHandler(WorkingDirRef workingDir)
    : m_pListener(nullptr)
    , m_WorkingDir(workingDir)
{
}

AttachmentHandler::~AttachmentHandler()
{
}

void AttachmentHandler::processFile(const std::string &file, long long freeSpace)
{
    m_CurrentFile = file;
    m_CurrentFreeSpace = freeSpace;
    ecore_thread_run(processRoutineCb, nullptr, nullptr, this);
}

void AttachmentHandler::setListener(IAttachmentHandlerListener *listener)
{
    m_pListener = listener;
}

void AttachmentHandler::processRoutineCb(void *data, Ecore_Thread *tread)
{
    TRACE;
    AttachmentHandler* self = static_cast<AttachmentHandler*>(data);
    self->onRoutine();
}

void *AttachmentHandler::safeCallOnFileReady(void *data)
{
    AttachmentHandler* self = static_cast<AttachmentHandler*>(data);
    self->m_pListener->onFileReady(self->m_CurrentFile);
    return nullptr;
}

void *AttachmentHandler::safeCallOnFileFails(void *data)
{
    AttachmentHandler* self = static_cast<AttachmentHandler*>(data);
    self->m_pListener->onFileFails();
    return nullptr;
}

void AttachmentHandler::onRoutine()
{
    if(!FileUtils::isExists(m_CurrentFile) || FileUtils::isDir(m_CurrentFile))
    {
        MSG_LOG_ERROR("File not exists: ", m_CurrentFile);
        ecore_main_loop_thread_safe_call_sync(safeCallOnFileFails, this);
        return;
    }

    m_CurrentFile = m_WorkingDir->addFile(m_CurrentFile);
    if(m_CurrentFile.empty())
    {
        MSG_LOG_ERROR("Error while copy file");
        ecore_main_loop_thread_safe_call_sync(safeCallOnFileFails, this);
        return;
    }

    if(FileUtils::getFileSize(m_CurrentFile) > m_CurrentFreeSpace)
    {
        if(getMediaType(m_CurrentFile).type == MsgMedia::ImageType && MediaUtils::downgradeImageQuality(m_CurrentFile) > m_CurrentFreeSpace)
        {
            m_WorkingDir->removeFile(m_CurrentFile);
            ecore_main_loop_thread_safe_call_sync(safeCallOnFileFails, this);
            return;
        }
    }

    ecore_main_loop_thread_safe_call_sync(safeCallOnFileReady, this);
}
