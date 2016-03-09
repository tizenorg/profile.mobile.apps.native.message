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

#include "MsgEngine.h"
#include "Logger.h"

#ifdef TIZEN_PRIVATE_API
#include "private/MsgStoragePrivate.h"
#include "private/MsgTransportPrivate.h"
#include "private/MsgSettingsPrivate.h"
#include "private/MsgComposerPrivate.h"
#else
 // TODO: msg public headers
#endif

#include <assert.h>

using namespace Msg;

const std::string handleIsNullStr = "MsgHandle is null!";
const std::string notImplPublicStr = "Msg. public API is not implemented";

MsgEngine::MsgEngine()
    : m_MsgHandle(nullptr)
{
}

MsgEngine::~MsgEngine()
{
    closeService();
}

int MsgEngine::openService()
{
    TRACE;
    if(m_MsgHandle)
    {
        MSG_LOG_ERROR("Handle already has been opened");
    }
    int result = 0;


#ifdef TIZEN_PRIVATE_API
    int res = msg_open_msg_handle(&m_MsgHandle);
    MSG_LOG_ERROR("handle open error = ", res);

    m_Storage.reset(new MsgStoragePrivate(m_MsgHandle));
    m_Transport.reset(new MsgTransportPrivate(m_MsgHandle));
    m_Settings.reset(new MsgSettingsPrivate(m_MsgHandle));
    m_Composer.reset(new MsgComposerPrivate(m_MsgHandle));
#else
    // TODO: impl for public API

	result = messages_open_service(&m_MsgHandle);

	/*	if(result == MESSAGES_ERROR_NONE)
	{
		m_Storage.reset(new MsgStorage(m_ServiceHandle));
	}*/
#endif

    return result;
}

int MsgEngine::closeService()
{
    int result = 0;
    if(m_MsgHandle)
    {
#ifdef TIZEN_PRIVATE_API
        msg_close_msg_handle(&m_MsgHandle);
#else
        result = messages_close_service(m_MsgHandle);
#endif

        m_MsgHandle = nullptr;
    }
    return result;
}

bool MsgEngine::isReady(std::string &errorMsg) const
{
#ifdef TIZEN_PRIVATE_API
    if(!m_MsgHandle)
    {
        errorMsg = handleIsNullStr;
        return false;
    }
#else
    errorMsg = notImplPublicStr;
    return false;
#endif
    return true;
}

MsgStorage &MsgEngine::getStorage()
{
    assert(m_Storage.get());
    return *m_Storage;
}

const MsgStorage &MsgEngine::getStorage() const
{
    assert(m_Storage.get());
    return *m_Storage;
}

MsgTransport &MsgEngine::getTransport()
{
    assert(m_Transport.get());
    return *m_Transport;
}

const MsgTransport &MsgEngine::getTransport() const
{
    assert(m_Transport.get());
    return *m_Transport;
}

MsgSettings &MsgEngine::getSettings()
{
    assert(m_Settings.get());
    return *m_Settings;
}

const MsgSettings &MsgEngine::getSettings() const
{
    assert(m_Settings.get());
    return *m_Settings;
}

MsgComposer &MsgEngine::getComposer()
{
    assert(m_Composer.get());
    return *m_Composer;
}

const MsgComposer &MsgEngine::getComposer() const
{
    assert(m_Composer.get());
    return *m_Composer;
}

std::string MsgEngine::whatError(int error)
{
#ifdef TIZEN_PRIVATE_API
	// TODO: impl for private API
#else
    switch(error)
    {
        case MESSAGES_ERROR_NONE:
            return "MESSAGES_ERROR_NONE";
        break;
        case MESSAGES_ERROR_OUT_OF_MEMORY:
            return "MESSAGES_ERROR_OUT_OF_MEMORY";
        break;
        case MESSAGES_ERROR_INVALID_PARAMETER:
            return "MESSAGES_ERROR_INVALID_PARAMETER";
        break;
        case MESSAGES_ERROR_SERVER_NOT_READY:
            return "MESSAGES_ERROR_SERVER_NOT_READY";
        break;
        case MESSAGES_ERROR_COMMUNICATION_WITH_SERVER_FAILED:
            return "MESSAGES_ERROR_COMMUNICATION_WITH_SERVER_FAILED";
        break;
        case MESSAGES_ERROR_OUT_OF_RANGE:
            return "MESSAGES_ERROR_OUT_OF_RANGE";
        break;
        case MESSAGES_ERROR_SENDING_FAILED:
            return "MESSAGES_ERROR_SENDING_FAILED";
        break;
        case MESSAGES_ERROR_OPERATION_FAILED:
            return "MESSAGES_ERROR_OPERATION_FAILED";
        break;
        case MESSAGES_ERROR_NO_SIM_CARD:
            return "MESSAGES_ERROR_NO_SIM_CARD";
        break;
        case MESSAGES_ERROR_NO_DATA:
            return "MESSAGES_ERROR_NO_DATA";
        break;
        case MESSAGES_ERROR_PERMISSION_DENIED:
            return "MESSAGES_ERROR_PERMISSION_DENIED";
        break;
        case MESSAGES_ERROR_NOT_SUPPORTED:
            return "MESSAGES_ERROR_NOT_SUPPORTED";
        break;
        default:
            return "SOME ANOTHER ERROR";
            break;
    }

#endif
    return "SOME ANOTHER ERROR";
}

void MsgEngine::calculateTextMetric(const std::string &text, MsgTextMetric &textMetric)
{
#ifdef TIZEN_PRIVATE_API
    const int maxSegments = 3;
    const int maxGsm7Len = 160;
    const int maxUnicodeLen = 70;

    msg_encode_type_t encode = MSG_ENCODE_GSM7BIT;
    unsigned textLen = 0;
    unsigned segmentLen = 0;
    int bytesInChar = 1;
    textMetric.reset();

    if(!text.empty())
    {
        if(msg_util_calculate_text_length(text.c_str(), MSG_ENCODE_AUTO, &textLen, &segmentLen, &encode) != 0)
        {
            MSG_LOG_ERROR("msg_util_calculate_text_length returns error");
        }

        textMetric.bytes = textLen;

        switch(encode)
        {
            case MSG_ENCODE_UCS2:
                bytesInChar = 2;
                break;

            case MSG_ENCODE_GSM7BIT:
            case MSG_ENCODE_AUTO:
                bytesInChar = 1;
                break;

            default:
                MSG_LOG_ERROR("Unknown encode type: ", encode);
                break;
        }

        textLen /= bytesInChar;
        segmentLen /= bytesInChar;
    }

    if(segmentLen == 0)
        segmentLen = encode == MSG_ENCODE_UCS2 ? maxUnicodeLen : maxGsm7Len;

    if(textLen == 0)
        textLen = text.length() / bytesInChar;

    textMetric.segmentsCount = (textLen / (segmentLen + 1)) + 1;

    if(textLen > 0)
        textMetric.charsLeft = segmentLen - (textLen % (segmentLen + 1));
    else
        textMetric.charsLeft = maxGsm7Len;

    textMetric.isMms = textMetric.segmentsCount > maxSegments;
#endif
}
