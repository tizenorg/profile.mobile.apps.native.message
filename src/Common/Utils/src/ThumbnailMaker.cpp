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

#include "ThumbnailMaker.h"
#include "PathUtils.h"
#include "Logger.h"
#include "App.h"
#include <Elementary.h>
#include <stdlib.h>

using namespace Msg;

const int defaultThumbSize = 74;
const int defaultCache = 512;
#define COLOR_BLUE 61, 185, 204, 255
#define RAND (rand() % 220)
#define COLOR_RAND RAND, RAND, RAND, 255
#define MSG_THUMB_STYLE_LIST    "list_ic_user_thumb_masking"

ThumbnailMaker::ThumbnailMaker(App &app)
    : m_App(app)
    , m_ContactsMap()
    , m_OriginsMap()
    , m_CurrentId()
{
    m_App.getContactManager().addListener(*this);
    m_ContactsMap.reserve(defaultCache);
    m_OriginsMap.reserve(defaultCache);
    for(int i = 0; i < MaxDefaultThumb; i++)
    {
        getThumbId((DefaultThumbs)i);
    }
}

ThumbnailMaker::~ThumbnailMaker()
{
    m_App.getContactManager().removeListener(*this);
}

ThumbnailMaker::ThumbId ThumbnailMaker::getThumbId(const MsgAddress &address)
{
    return getThumbId(address.getAddress());
}

ThumbnailMaker::ThumbId ThumbnailMaker::getThumbId(const std::string &address)
{
    ContactPersonAddressRef contact = m_App.getContactManager().getContactPersonAddress(address);
    return contact ? getThumbIdFromFile(contact->getThumbnailPath()) : getThumbId(SingleThumb);
}

ThumbnailMaker::ThumbId ThumbnailMaker::getThumbIdFromFile(const std::string &path)
{
    if(path.empty())
    {
        return getThumbId(SingleThumb);
    }
    else
    {
        auto it = m_ContactsMap.find(path);
        if(it != m_ContactsMap.end())
        {
            return it->second;
        }
        Evas_Object *origin = makeOriginThumb(m_App.getWindow(), path);
        m_OriginsMap[++m_CurrentId] = origin;
        m_ContactsMap[path] = m_CurrentId;
        return m_CurrentId;
    }
}

ThumbnailMaker::ThumbId ThumbnailMaker::getThumbId(DefaultThumbs thumb)
{
    auto it = m_ContactsMap.find(defaultThumbsToStr(thumb));
    if(it != m_ContactsMap.end())
    {
        return it->second;
    }
    else
    {
        Evas_Object *origin = nullptr;
        if(thumb != OwnerThumb)
        {
            origin = makeDefaultOriginThumb(m_App.getWindow(), defaultThumbsToPath(thumb));
        }
        else
        {
            auto ownerProfile = m_App.getContactManager().getOwnerProfile();
            if(ownerProfile)
            {
                origin = makeOriginThumb(m_App.getWindow(), ownerProfile->getThumbnailPath());
            }
            else
            {
                origin = makeDefaultOriginThumb(m_App.getWindow(), defaultThumbsToPath(SingleThumb));
            }

        }
        m_OriginsMap[++m_CurrentId] = origin;
        m_ContactsMap[defaultThumbsToStr(thumb)] = m_CurrentId;
        return m_CurrentId;
    }
}

Evas_Object *ThumbnailMaker::getThumbById(Evas_Object *parent, ThumbId id)
{
    Evas_Object *ic = nullptr;
    Evas_Object *origin = m_OriginsMap[id];
    ic = evas_object_image_filled_add(evas_object_evas_get(parent));
    evas_object_image_source_set(ic, origin);
    evas_object_size_hint_min_set(ic, ELM_SCALE_SIZE(defaultThumbSize), ELM_SCALE_SIZE(defaultThumbSize));
    evas_object_size_hint_max_set(ic, ELM_SCALE_SIZE(defaultThumbSize), ELM_SCALE_SIZE(defaultThumbSize));
    View::expand(ic);
    evas_object_show(ic);
    return ic;
}

Evas_Object *ThumbnailMaker::makeOriginThumb(Evas_Object *parent, const std::string &path)
{
    Evas_Object *ic = elm_layout_add(parent);
    std::string edjePath = PathUtils::getResourcePath(THUMBNAIL_EDJ_PATH);
    elm_layout_file_set(ic, edjePath.c_str(), MSG_THUMB_STYLE_LIST);

    Evas_Object *img = makeFace(ic, path);
    elm_object_part_content_set(ic, "content", img);
    evas_object_resize(ic, ELM_SCALE_SIZE(defaultThumbSize), ELM_SCALE_SIZE(defaultThumbSize));
    evas_object_move(ic, -ELM_SCALE_SIZE(defaultThumbSize), -ELM_SCALE_SIZE(defaultThumbSize));
    evas_object_show(ic);
    return ic;
}

Evas_Object *ThumbnailMaker::makeDefaultOriginThumb(Evas_Object *parent, const std::string &path)
{
    Evas_Object *img = makeFace(parent, path);
    evas_object_color_set(img, COLOR_BLUE);
    evas_object_resize(img, ELM_SCALE_SIZE(defaultThumbSize), ELM_SCALE_SIZE(defaultThumbSize));
    evas_object_move(img, -ELM_SCALE_SIZE(defaultThumbSize), -ELM_SCALE_SIZE(defaultThumbSize));
    evas_object_show(img);
    return img;
}

Evas_Object *ThumbnailMaker::makeFace(Evas_Object *parent, const std::string &path)
{
    Evas_Object *img = elm_image_add(parent);
    elm_image_file_set(img, path.c_str(), nullptr);
    evas_object_size_hint_min_set(img, ELM_SCALE_SIZE(defaultThumbSize), ELM_SCALE_SIZE(defaultThumbSize));
    evas_object_size_hint_max_set(img, ELM_SCALE_SIZE(defaultThumbSize), ELM_SCALE_SIZE(defaultThumbSize));
    elm_image_aspect_fixed_set(img, EINA_TRUE);
    elm_image_fill_outside_set(img, EINA_TRUE);
    return img;
}

void ThumbnailMaker::invalidate()
{
    MSG_LOG("");
    //TODO: make update on contacts changed cb
    for(auto it: m_OriginsMap)
    {
        if(it.first >= MaxDefaultThumb)
            evas_object_del(it.second);
    }
    m_OriginsMap.clear();
    m_ContactsMap.clear();
}

const std::string &ThumbnailMaker::defaultThumbsToStr(DefaultThumbs thumb)
{
    static const std::string sThumb = "SingleThumb";
    static const std::string gThumb = "GroupThumb";
    static const std::string oThumb = "OwnerThumb";
    static const std::string emptyThumb = "";
    switch (thumb)
    {
        case SingleThumb:
            return sThumb;
        case GroupThumb:
            return gThumb;
        case OwnerThumb:
            return oThumb;
        default :
            return emptyThumb;
    }
}

std::string ThumbnailMaker::defaultThumbsToPath(DefaultThumbs thumb)
{
    switch (thumb)
    {
        case SingleThumb:
            return PathUtils::getResourcePath(THUMB_CONTACT_IMG_PATH);
        case GroupThumb:
            return PathUtils::getResourcePath(THUMB_GROUP_IMG_PATH);
        default :
            return "";
    }
}

void ThumbnailMaker::onContactChanged()
{
    invalidate();
}
