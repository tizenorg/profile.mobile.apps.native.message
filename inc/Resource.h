/*
 * Copyright (c) 2014 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
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

#ifndef RESOURCE_H_
#define RESOURCE_H_

#define EDJ_PATH "edje"
#define IMAGE_PATH "images"

// Edj images:
#define CONV_SEND_IMG                   "btn_conversation_message_sending.png"
#define CONV_ADD_IMG                    "btn_conversation_add.png"
#define CONTACT_IMG                     "icon_btn_contact.png"
#define SOFT_SEARCH_BACK_IMG            "icon_soft_search_back.png"

// Edj:
#define IMAGES_EDJ_PATH                  EDJ_PATH"/images.edj"
#define BUTTON_THEME_EDJ_PATH            EDJ_PATH"/button_theme.edj"

#define BUBBLE_THEME_EDJ_PATH            EDJ_PATH"/bubble_theme.edj"
#define MSG_INPUT_PANEL_EDJ_PATH         EDJ_PATH"/msg_input_panel.edj"
#define RECIPIENT_PANEL_EDJ_PATH         EDJ_PATH"/recipient_panel.edj"
#define CONV_LAYOUT_EDJ_PATH             EDJ_PATH"/conv_layout.edj"
#define MSG_THREAD_EDJ_PATH              EDJ_PATH"/msg_thread.edj"
#define CONTACTS_SEARCH_EDJ_PATH         EDJ_PATH"/contact_search.edj"
#define MSG_BODY_EDJ_PATH                EDJ_PATH"/msg_body.edj"
#define THUMBNAIL_EDJ_PATH               EDJ_PATH"/msg_thumbnail.edj"
#define MSG_TITLE_EDJ_PATH               EDJ_PATH"/msg_title.edj"
#define CONV_LIST_EDJ_PATH               EDJ_PATH"/conv_list.edj"
#define SELECT_ALL_EDJ_PATH              EDJ_PATH"/select_all.edj"

// Images(res):
#define THUMB_CONTACT_IMG_PATH           IMAGE_PATH"/list_ic_contacts.png"
#define THUMB_GROUP_IMG_PATH             IMAGE_PATH"/list_ic_group.png"
#define THUMB_CB_IMG_PATH                IMAGE_PATH"/list_ic_cb_message.png"
#define THUMB_PUSH_IMG_PATH              IMAGE_PATH"/list_ic_push_message.png"
#define THUMB_OTA_IMG_PATH               IMAGE_PATH"/list_ic_OTA_message.png"
#define THUMB_USER_MASK_74x74_IMG_PATH   IMAGE_PATH"/list_ic_user_thumb_masking.png"
#define MSG_THREAD_FLOATING_BTN_ICON     IMAGE_PATH"/message_floating_button_icon.png"
#define TEST_IMG_PATH                    IMAGE_PATH"/cat.jpeg"

// Colors:
#define BUTTON_COLOR                     2, 61, 132, 255
#define BUTTON_PRESSED_COLOR             1, 37, 79, 255
#define BUTTON_DISABLED_COLOR            179, 197, 218, 255

#endif /* RESOURCE_H_ */
