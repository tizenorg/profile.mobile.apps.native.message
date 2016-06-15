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

#include "SmilVideoItemView.h"
#include "Resource.h"
#include "Logger.h"
using namespace Msg;

namespace
{
    const int viewerCanvasWidth = 656;
    const int videoWidth = 118;
    const int videoHeight = 144;
    const int videoCanvasHeightPortrait = videoWidth * 2; // QCIF width(176) size * 2
    const int videoCanvasHeightLandscape = videoHeight * 2;  // QCIF height(144) size * 2

    // TODO: This func. port from old message application, check it.
    void getResizedSize(int contentW, int contentH, int &w, int &h)
    {
        int canvasWidth = viewerCanvasWidth;
        int canvasHeight = (contentW > contentH) ? videoCanvasHeightLandscape : videoCanvasHeightPortrait;

       // Check image size
        if(contentW > canvasWidth)
        {
            w = canvasWidth;
            h = contentH * canvasWidth / contentW;

            if (h > canvasHeight)
            {
                w = contentW * canvasHeight / contentH;
                h = canvasHeight;
            }
        }
        else if(contentH > canvasHeight)
        {
            w = contentW * canvasHeight / contentH;
            h = canvasHeight;

            if (w > canvasWidth)
            {
                w = canvasWidth;
                h = contentH * canvasWidth / contentW;
            }
        }
        else if(contentW <= canvasWidth && contentH <= canvasHeight)
        {
            w = contentW * canvasHeight / contentH;
            h = canvasHeight;
        }
        else
        {
            w = contentW;
            h = contentH;
        }

        // Uncomment if needed:
        #if 0
        // Make size as even number becasue of hardware limitation
        if(w % 2)
         w = w - 1;

        if (h % 2)
         h = h - 1;

        // Make size as multiples of 16 because of hardware limitation, C110
        int remained = 0;
        if((remained = w % 16))
            w = w - remained;

        if((remained = h % 16))
            h = h - remained;
        #endif
    }
}

SmilVideoItemView::SmilVideoItemView(Evas_Object *parent, int videoWidth, int videoHeight)
    : m_pVideoSink(nullptr)
{
    int w = 0;
    int h = 0;

    if(videoWidth > 0 && videoHeight > 0)
        getResizedSize(videoWidth, videoHeight, w, h);

    setEo(addLayout(parent, SMIL_ITEM_EDJ_PATH, "smil_video_item"));
    Evas_Object *image = createImage(getEo(), w, h);
    setContent(image, "swallow.thumbnail");
}

SmilVideoItemView::~SmilVideoItemView()
{

}

Evas_Object *SmilVideoItemView::getVideoSink() const
{
    return m_pVideoSink;
}

Evas_Object *SmilVideoItemView::createImage(Evas_Object *parent, int width, int height)
{
    Evas *evas = evas_object_evas_get(parent);
    m_pVideoSink = evas_object_image_filled_add(evas);
    expand(m_pVideoSink);
    evas_object_size_hint_min_set(m_pVideoSink, width, height);
    evas_object_size_hint_max_set(m_pVideoSink, width, height);
    evas_object_show(m_pVideoSink);
    return m_pVideoSink;
}

