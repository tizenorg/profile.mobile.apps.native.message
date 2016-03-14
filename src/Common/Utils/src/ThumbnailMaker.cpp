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
}

ThumbnailMaker::~ThumbnailMaker()
{
    m_App.getContactManager().removeListener(*this);
}

ThumbnailMaker::ThumbId ThumbnailMaker::makeThumbFromAddress(const MsgAddress &address)
{
    return makeThumbFromAddress(address.getAddress());
}

ThumbnailMaker::ThumbId ThumbnailMaker::makeThumbFromAddress(const std::string &address)
{
    ContactPersonAddressRef contact = m_App.getContactManager().getContactPersonAddress(address);
    if(contact)
        return makeThumbFromFile(contact->getThumbnailPath());
    else
        return makeDefaultThumb(SingleThumb);
}

ThumbnailMaker::ThumbId ThumbnailMaker::makeThumbFromFile(const std::string &path)
{
    if(path.empty())
    {
        return makeDefaultThumb(SingleThumb);
    }
    else
    {
        auto it = m_ContactsMap.find(path);
        if(it != m_ContactsMap.end())
        {
            return it->second;
        }
        Evas_Object *origin = makeOriginThumb(m_App.getWindow().getWinEvasObject(), path);
        m_OriginsMap[++m_CurrentId] = origin;
        m_ContactsMap[path] = m_CurrentId;
        return m_CurrentId;
    }
}

ThumbnailMaker::ThumbId ThumbnailMaker::makeDefaultThumb(DefaultThumbs thumb)
{
    auto it = m_ContactsMap.find(DefaultThumbsToStr(thumb));
    if(it != m_ContactsMap.end())
    {
        return it->second;
    }
    else
    {
        Evas_Object *origin = nullptr;
        if(thumb != OwnerThumb)
        {
            origin = makeDefaultOriginThumb(m_App.getWindow().getWinEvasObject(), DefaultThumbsToPath(thumb));
        }
        else
        {
            auto ownerProfile = m_App.getContactManager().getOwnerProfile();
            if(ownerProfile)
            {
                origin = makeOriginThumb(m_App.getWindow().getWinEvasObject(), ownerProfile->getThumbnailPath());
            }
            else
            {
                origin = makeDefaultOriginThumb(m_App.getWindow().getWinEvasObject(), DefaultThumbsToPath(SingleThumb));
            }

        }
        m_OriginsMap[++m_CurrentId] = origin;
        m_ContactsMap[DefaultThumbsToStr(thumb)] = m_CurrentId;
        return m_CurrentId;
    }
}

Evas_Object *ThumbnailMaker::getThumbById(Evas_Object *parent, ThumbId id)
{
    Evas_Object *ic = nullptr;
    Evas_Object *origin = m_OriginsMap[id];
    MSG_LOG("ThumbId: ", id);
    MSG_LOG("Origin: ", origin)
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
    Evas_Object *ic = nullptr;

    ic = elm_layout_add(parent);
    std::string edjePath = PathUtils::getResourcePath(THUMBNAIL_EDJ_PATH);
    elm_layout_file_set(ic, edjePath.c_str(), MSG_THUMB_STYLE_LIST);

    Evas_Object *img = elm_image_add(ic);
    elm_image_file_set(img, path.c_str(), nullptr);
    evas_object_size_hint_min_set(img, ELM_SCALE_SIZE(defaultThumbSize), ELM_SCALE_SIZE(defaultThumbSize));
    evas_object_size_hint_max_set(img, ELM_SCALE_SIZE(defaultThumbSize), ELM_SCALE_SIZE(defaultThumbSize));
    elm_image_aspect_fixed_set(img, EINA_TRUE);
    elm_image_fill_outside_set(img, EINA_TRUE);
    elm_object_part_content_set(ic, "content", img);
    evas_object_resize(ic, ELM_SCALE_SIZE(defaultThumbSize), ELM_SCALE_SIZE(defaultThumbSize));
    evas_object_move(ic, -ELM_SCALE_SIZE(defaultThumbSize), -ELM_SCALE_SIZE(defaultThumbSize));
    evas_object_show(ic);

    return ic;
}

Evas_Object *ThumbnailMaker::makeDefaultOriginThumb(Evas_Object *parent, const std::string &path)
{
    Evas_Object *img = elm_image_add(parent);
    elm_image_file_set(img, path.c_str(), nullptr);
    evas_object_size_hint_min_set(img, ELM_SCALE_SIZE(defaultThumbSize), ELM_SCALE_SIZE(defaultThumbSize));
    evas_object_size_hint_max_set(img, ELM_SCALE_SIZE(defaultThumbSize), ELM_SCALE_SIZE(defaultThumbSize));
    elm_image_aspect_fixed_set(img, EINA_TRUE);
    elm_image_fill_outside_set(img, EINA_TRUE);
    evas_object_color_set(img, COLOR_BLUE);
    evas_object_resize(img, ELM_SCALE_SIZE(defaultThumbSize), ELM_SCALE_SIZE(defaultThumbSize));
    evas_object_move(img, -ELM_SCALE_SIZE(defaultThumbSize), -ELM_SCALE_SIZE(defaultThumbSize));
    evas_object_show(img);
    return img;
}

void ThumbnailMaker::invalidate()
{
    MSG_LOG("");
    //TODO: make update on contacts changed cb
}

std::string ThumbnailMaker::DefaultThumbsToStr(DefaultThumbs thumb)
{
    switch (thumb)
    {
        case SingleThumb:
            return "SingleThumb";
        case GroupThumb:
            return "GroupThumb";
        case OwnerThumb:
            return "OwnerThumb";
        default :
            return "";
    }
}

std::string ThumbnailMaker::DefaultThumbsToPath(DefaultThumbs thumb)
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
