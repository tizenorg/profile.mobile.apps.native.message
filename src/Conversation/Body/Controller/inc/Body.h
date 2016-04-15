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

#ifndef Body_h_
#define Body_h_

#include "BodyView.h"
#include "MessageSMS.h"
#include "MessageMms.h"
#include "WorkingDir.h"
#include "AppControlCompose.h"
#include "Page.h"

#include <list>
#include <Ecore.h>

namespace Msg
{
    class IBodyListener;
    class App;
    class PopupListItem;
    class PopupList;

    // TODO: move page(controller) methods to Page class

    class Body
        : public BodyView
    {
        public:
            Body(App &app, WorkingDirRef workingDir);
            virtual ~Body();

            void create(Evas_Object *parent);
            void setListener(IBodyListener *listener);

            bool addMedia(const std::list<std::string> &fileList);
            bool addMedia(const std::string &filePath);

            bool isMms();
            void setMmsRecipFlag(bool value);
            const MsgTextMetric &getTextMetric();
            long long getMsgSize();
            int getAttachmentsCountTotal() const;
            void read(Message &msg);
            void write(const Message &msg);
            void execCmd(const AppControlComposeRef &cmd);
            Page &getDefaultPage();

        private:
            Page &createPage();
            void showTooLargePopup();
            void showTooMuchAttachedPopup(int willBeAttached);
            void showTooMuchAttachedPopup();
            void read(MessageSMS &msg);
            void read(MessageMms &msg);
            void readAttachments(MessageMms &msg);
            void write(const MessageSMS &msg);
            void write(const MessageMms &msg);
            void writeAttachments(const MessageMms &msg);
            void writeTextToFile(TextPageViewItem &item);
            void addAttachment( const std::string &filePath, const std::string &fileName = "");

            void onTooLargePopupDel(Evas_Object *obj, void *eventInfo);
            void onTooMuchAttachedPopupDel(Evas_Object *obj, void *eventInfo);

            std::string createVcfFile(const AppControlComposeRef &cmd);

            // BodyView:
            virtual void onContentChanged();
            virtual void onItemDelete(PageViewItem &item);
            virtual void onItemDelete(BodyAttachmentViewItem &item);

            //IMediaPageViewItemListener
            virtual void onClicked(MediaPageViewItem &item);

            //IBodyAttachmentViewListener
            virtual void onClicked(BodyAttachmentViewItem &item);

            PopupList &createPopupList(const std::string &title);
            void onRemoveMediaItemPressed(PopupListItem &item);
            void onRemoveBodyItemPressed(PopupListItem &item);
        private:
            IBodyListener *m_pListener;
            App &m_App;
            WorkingDirRef m_WorkingDir;
            Ecore_Idler *m_pOnChangedIdler;
            bool m_TooLargePopupShow;
            bool m_TooMuchAttachedPopupShow;
            bool m_MmsRecipFlag;
    };

    class IBodyListener
    {
        public:
            virtual ~IBodyListener() {}

            virtual void onChanged(Body &body) {};
    };
}

#endif /* Body_h_ */
