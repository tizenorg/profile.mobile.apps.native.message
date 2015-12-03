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
 */

#ifndef _MESSAGE_DEF_PRIVATE_H__
#define _MESSAGE_DEF_PRIVATE_H__

#include <string>

namespace Msg
{
    // SMIL layout:
    const int smilScreenWidth  = 480;
    const int smilScreenHeight = 800;

    const int smilPosTop       = 1;
    const int smilPosBottom    = 50;
    const int smilPosLeft      = 0;

    const int smilRegionWidth  = 100;
    const int smilRegionHeight = 50;

    // Text style:
    const int smilDefaultBgColor   = 0xFF000000;
    const int smilDefaultFontColor = 0xFFFFFFFF;
    const int smilDefaultFontSize  = 30;
    const bool smilDefaultBold = false;
    const bool smilDefaultUnderline = false;

    // Region id:
    const std::string textRegionId = "Text";
    const std::string imageRegionId = "Image";

    // Mime:
    const std::string mimeText = "text/plain";

}

#endif /* _MESSAGE_DEF_PRIVATE_H__ */
