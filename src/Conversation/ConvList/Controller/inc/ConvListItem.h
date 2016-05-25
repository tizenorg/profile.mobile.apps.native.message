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

#ifndef ConvListItem_H_
#define ConvListItem_H_

#include "ConvListViewItem.h"
#include "MsgTypes.h"
#include "BubbleView.h"
#include "ThumbnailMaker.h"
#include "Message.h"
#include "ContextPopup.h"
#include "App.h"
#include <string>
#include "MessageDetailContent.h"
#include "FileViewer.h"
#include "MsgUtils.h"

namespace Msg
{
    class MsgConversationItem;
    class IConvListItemListener;

    class ConvListItem
        : public ConvListViewItem
        , public IBubbleViewListener
    {
        public:
            /**
             * @brief Creates item for Conversation list
             * @param[in] item MsgConversationItem model
             * @param[in] app Main application
             * @param[in] workingDir Smart ptr to current working dir.
             * @param[in] searchWord string for search in bubble
             * @param[in] thumbId Reference to id of user thumbnail
             */
            ConvListItem(const MsgConversationItem &item,
                         App &app,
                         FileViewer &fileViewer,
                         WorkingDirRef workingDir,
                         const std::string &searchWord,
                         const ThumbnailMaker::ThumbId &thumbId = -1);

            virtual ~ConvListItem();

            /**
             * @brief Returns MsgId related to this ConvListItem
             */
            MsgId getMsgId() const;

            /**
             * @brief Returns message's time
             */
            time_t getRawTime() const;
            void showPopup();
            void setListener(IConvListItemListener *l);

            void updateStatus();
            void updateTime();

        protected:
            // ConvListViewItem:
            virtual Evas_Object *getBubbleContent();
            virtual Evas_Object *getThumbnail();
            virtual Evas_Object *getProgress();
            virtual std::string getTime();
            virtual std::string getMsgType();

            virtual void onEditButtonClicked(Evas_Object *obj, void *event_info);
            virtual void onFailedButtonClicked(Evas_Object *obj, void *event_info);

        private:
            ConvListViewItem::ConvItemType getConvItemType(const MsgConversationItem &item);
            void prepareBubble(const MsgConversationItem &item, const std::string &searchWord);
            void addVideoItem(const MsgConvMedia &media);
            void addAudioItem(const MsgConvMedia &media);
            void addTextItem(const MsgConvMedia &media, const std::string &searchWord);
            void addImageItem(const MsgConvMedia &media);
            void addAttachedFileItem(const MsgConvMedia &media);

            // Create Popup when message is clicked
            void showMainCtxPopup();
            void showDraftCtxPopup();
            void onDownloadItemPressed(ContextPopupItem &item);
            void onDeleteItemPressed(ContextPopupItem &item);
            void onCopyTextItemPressed(ContextPopupItem &item);
            void onForwardItemPressed(ContextPopupItem &item);
            void onResendItemPressed(ContextPopupItem &item);
            void onSlideShowItemPressed(ContextPopupItem &item);
            void onEditItemPressed(ContextPopupItem &item);
            void onSaveAttachmentsItemPressed(ContextPopupItem &item);
            void onCopyToSimCardItemPressed(ContextPopupItem &item);
            void onViewDetailsItemPressed(ContextPopupItem &item);
            std::string getAllMsgText() const;

            // Create popup when failed button is clicked
            void showFailedToSendPopup();
            void onCancelButtonClicked(Popup &popup, int buttonId);
            void onFailedResendButtonClicked(Popup &popup, int buttonId);
            void onDeleteButtonClicked(Popup &popup, int buttonId);
            void onPopupDel(Evas_Object *popup, void *eventInfo);

            // IBubbleViewListener
            virtual void onDownloadButtonClicked();
            virtual void onItemClicked(BubbleEntity::Item &item);

        private:
            IConvListItemListener *m_pListener;
            App &m_App;
            WorkingDirRef m_WorkingDir;
            FileViewer &m_FileViewer;
            MsgId m_MsgId;
            bool m_IsDraft;
            Message::NetworkStatus m_NetworkStatus;
            Message::Type m_Type;
            time_t m_Time;
            std::string m_TimeStr;
            BubbleEntity m_BubbleEntity;
            const ThumbnailMaker::ThumbId &m_ThumbId;
    };

    class IConvListItemListener
    {
        public:
            virtual ~IConvListItemListener() {}
            virtual void onEditDraftMsg(ConvListItem &item) {};
            virtual void onForwardMsg(ConvListItem &item) {};
            virtual void onSlideShow(ConvListItem &item) {};
    };
}


#endif /* ConvListItem_H_ */
