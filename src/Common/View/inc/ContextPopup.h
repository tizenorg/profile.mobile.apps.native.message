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

#ifndef ContextPopup_h_
#define ContextPopup_h_

#include "View.h"
#include "ViewItem.h"
#include "HwButtonListener.h"

#include <Elementary.h>
#include <string>

namespace Msg
{
    class IContextPopupListener;
    class ContextPopup;
    class ContextPopupItem;
    class Window;

    typedef void (*ContextPopupItemPressedCb)(ContextPopup &v, ContextPopupItem &item, void *userData);

    class ContextPopupItem
        : public ViewItem
    {
        friend class ContextPopup;

    public:
        ContextPopup &getParentContextPopup();
        int getId() const;

    private:
        ContextPopupItem(ContextPopup &parent, int id, ContextPopupItemPressedCb userCb, void *userData);
        virtual ~ContextPopupItem();

    private:
        ContextPopup &m_ParentContextPopup;
        int m_Id;
        ContextPopupItemPressedCb m_pUserCb;
        void *m_pUserData;
    };

    class ContextPopup
        : public View
        , private IHwButtonListener
    {
    public:
        ContextPopup(Evas_Object *parent);
        virtual ~ContextPopup();

        void setUserType(int type);
        int getUserType() const;
        ContextPopupItem *appendItem(int id, const std::string &text, Evas_Object *icon = nullptr,
                                     ContextPopupItemPressedCb cb = nullptr, void *userData = nullptr);
        void setListener(IContextPopupListener *listener);
        void dismiss();
        void setDirectionPriority(Elm_Ctxpopup_Direction first, Elm_Ctxpopup_Direction second,
                                  Elm_Ctxpopup_Direction third, Elm_Ctxpopup_Direction fourth);
        void align(Window &win);

        Elm_Ctxpopup_Direction getDirection() const;

    private:
        // IHwButtonListener:
        virtual void onHwBackButtonClicked();
        virtual void onHwMoreButtonClicked();

    private:
        void createContextPopup(Evas_Object *parent);

        static void on_dismissed_cb(void *data, Evas_Object *obj, void *event_info);
        static void on_item_pressed_cb(void *data, Evas_Object * obj, void *event_info);

    private:
        IContextPopupListener *m_pListener;
        int m_Type;
    };

    class IContextPopupListener
    {
    public:
        virtual ~IContextPopupListener() {};
        virtual void onContextPopupItemPressed(ContextPopup &v, ContextPopupItem &item) {};
        virtual void onContextPopupDismissed(ContextPopup &v) {};
    };
}

#endif // ContextPopup_h_
