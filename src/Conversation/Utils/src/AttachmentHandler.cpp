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
#include <Ecore.h>

using namespace Msg;

AttachmentHandler::AttachmentHandler(WorkingDirRef workingDir)
    : m_pListener(nullptr)
    , m_WorkingDir(workingDir)
    , m_NeedTerminate(false)
{
}

AttachmentHandler::~AttachmentHandler()
{
    std::unique_lock<std::mutex> _lock(m_Mutex);
    m_NeedTerminate = true;
    _lock.unlock();

    m_Condition.notify_one();
    if(m_Thread.joinable())
        m_Thread.join();
}

void AttachmentHandler::processFile(const std::string &file)
{
    if(!m_Thread.joinable())
        m_Thread = std::thread(&AttachmentHandler::onRoutine, this);

    std::unique_lock<std::mutex> _lock(m_Mutex);
    m_Task.push(file);
    m_Condition.notify_one();
}

void AttachmentHandler::setListener(IAttachmentHandlerListener *listener)
{
    m_pListener = listener;
}

void *AttachmentHandler::safeCallOnFreeSpaceRequest(void *data)
{
    auto *self = static_cast<AttachmentHandler*>(data);
    if(self->m_pListener)
        self->m_CurrentFreeSpace = self->m_pListener->onFreeSpaceRequest();
    return nullptr;
}

void *AttachmentHandler::safeCallOnFileReady(void *data)
{
    auto *self = static_cast<AttachmentHandler*>(data);
    if(self->m_pListener)
        self->m_pListener->onFileReady(self->m_CurrentFile);
    return nullptr;
}

void *AttachmentHandler::safeCallOnFileFails(void *data)
{
    auto *self = static_cast<AttachmentHandler*>(data);
    if(self->m_pListener)
        self->m_pListener->onFileFails();
    return nullptr;
}

void AttachmentHandler::onRoutine()
{
    while(!m_NeedTerminate)
    {
        std::unique_lock<std::mutex> _lock(m_Mutex);
        if(m_Task.empty() && !m_NeedTerminate)
            m_Condition.wait(_lock);
        if(m_Task.empty() || m_NeedTerminate)
            continue;

        m_CurrentFile = m_Task.front();
        m_Task.pop();
        _lock.unlock();

        if(!FileUtils::isExists(m_CurrentFile) || FileUtils::isDir(m_CurrentFile))
        {
            MSG_LOG_ERROR("File not exists: ", m_CurrentFile);
            ecore_main_loop_thread_safe_call_sync(safeCallOnFileFails, this);
            continue;
        }

        m_CurrentFile = m_WorkingDir->addFile(m_CurrentFile);
        if(m_CurrentFile.empty())
        {
            MSG_LOG_ERROR("Error while copy file");
            ecore_main_loop_thread_safe_call_sync(safeCallOnFileFails, this);
            continue;
        }

        ecore_main_loop_thread_safe_call_sync(safeCallOnFreeSpaceRequest, this);
        if(FileUtils::getFileSize(m_CurrentFile) > m_CurrentFreeSpace)
        {
            if((getMediaType(m_CurrentFile).type == MsgMedia::ImageType && MediaUtils::downgradeImageQuality(m_CurrentFile) > m_CurrentFreeSpace)
                    || getMediaType(m_CurrentFile).type != MsgMedia::ImageType)
            {
                m_WorkingDir->removeFile(m_CurrentFile);
                ecore_main_loop_thread_safe_call_sync(safeCallOnFileFails, this);
                continue;
            }
        }

        ecore_main_loop_thread_safe_call_sync(safeCallOnFileReady, this);
    }
}
