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

const int defaultCache = 512;
#define DEFAULT_THUMB_SIZE ELM_SCALE_SIZE(80)
#define COLOR_BLUE 61, 185, 204, 255
#define RAND (rand() % 220)
#define COLOR_RAND RAND, RAND, RAND, 255
#define MSG_THUMB_STYLE_LIST    "list_ic_user_thumb_masking"


ThumbnailMaker::ThumbnailMaker(App &app)
    : m_App(app)
    , m_ContactsMap()
    , m_OriginsMap()
    , m_CurrentId(-1)
{
    m_App.getContactManager().addListener(*this);
    m_ContactsMap.reserve(defaultCache);
    m_OriginsMap.reserve(defaultCache);

    for(int i = 0; i <= maxStaticThumbId; i++)
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
    ContactAddressRef contact = m_App.getContactManager().getContactAddress(address);
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
            return it->second;

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
            ContactMyProfileRef ownerProfile = m_App.getContactManager().getOwnerProfile();
            origin = ownerProfile ? makeOriginThumb(m_App.getWindow(), ownerProfile->getThumbnailPath()) :
                                    makeDefaultOriginThumb(m_App.getWindow(), defaultThumbsToPath(SingleThumb));
        }
        m_OriginsMap[++m_CurrentId] = origin;
        m_ContactsMap[defaultThumbsToStr(thumb)] = m_CurrentId;
        return m_CurrentId;
    }
}

Evas_Object *ThumbnailMaker::getThumbById(Evas_Object *parent, ThumbId id, int thumbSize)
{
    auto it = m_OriginsMap.find(id);
    if(it == m_OriginsMap.end())
    {
        MSG_LOG_ERROR("Invalid thumbnail id");
        return nullptr;
    }

    Evas_Object *origin = it->second;
    Evas_Object *ic = evas_object_image_filled_add(evas_object_evas_get(parent));
    evas_object_image_source_set(ic, origin);
    int scaledThumbSize = ELM_SCALE_SIZE(thumbSize);
    evas_object_size_hint_min_set(ic, scaledThumbSize, scaledThumbSize);
    evas_object_size_hint_max_set(ic, scaledThumbSize, scaledThumbSize);
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
    evas_object_resize(ic, DEFAULT_THUMB_SIZE, DEFAULT_THUMB_SIZE);
    evas_object_move(ic, -DEFAULT_THUMB_SIZE, -DEFAULT_THUMB_SIZE);
    evas_object_show(ic);
    return ic;
}

Evas_Object *ThumbnailMaker::makeDefaultOriginThumb(Evas_Object *parent, const std::string &path)
{
    Evas_Object *img = makeFace(parent, path);
    evas_object_color_set(img, COLOR_BLUE);
    evas_object_resize(img, DEFAULT_THUMB_SIZE, DEFAULT_THUMB_SIZE);
    evas_object_move(img, -DEFAULT_THUMB_SIZE, -DEFAULT_THUMB_SIZE);
    evas_object_show(img);
    return img;
}

Evas_Object *ThumbnailMaker::makeFace(Evas_Object *parent, const std::string &path)
{
    Evas_Object *img = elm_image_add(parent);
    elm_image_file_set(img, path.c_str(), nullptr);
    evas_object_size_hint_min_set(img, DEFAULT_THUMB_SIZE, DEFAULT_THUMB_SIZE);
    evas_object_size_hint_max_set(img, DEFAULT_THUMB_SIZE, DEFAULT_THUMB_SIZE);
    elm_image_aspect_fixed_set(img, EINA_TRUE);
    elm_image_fill_outside_set(img, EINA_TRUE);
    return img;
}

void ThumbnailMaker::invalidate()
{
    MSG_LOG("");

    // m_OriginsMap:
    auto originsIt = m_OriginsMap.begin();

    for(; originsIt != m_OriginsMap.end();)
    {
        if(originsIt->first > maxStaticThumbId)
        {
            MSG_LOG(originsIt->first);
            evas_object_del(originsIt->second);
            originsIt = m_OriginsMap.erase(originsIt);
        }
        else
            ++originsIt;
    }

    // m_ContactsMap:
    auto contactsIt = m_ContactsMap.begin();
    for(; contactsIt != m_ContactsMap.end();)
    {
        if(contactsIt->second > maxStaticThumbId)
            contactsIt = m_ContactsMap.erase(contactsIt);
        else
            ++contactsIt;
    }
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
