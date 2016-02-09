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
 
 #include "macros.inl"

//Layout
#define MAIN_W 220
#define MAIN_W_HORIZONTAL 480

//Body attributes
#define BODY_PADDING_TOP_1_MIN 30
#define BODY_PADDING_TOP_2_MIN 15
#define BODY_PADDING_MID_1_MIN 6
#define BODY_PADDING_BOTTOM_MIN 12

#define SUBJECT_LOCK_WIDTH 80
#define SUBJECT_MIN_H 80
#define SUPPLEMENT_AREA_H 76
#define SUPPLEMENT_AUDIO_AREA_H 42
#define SUMMARY_AUDIO_AREA_H 46
#define PAGE_INDEX_MIN_W 64
#define DETAIL_PADDING_V_MIN 14
#define DETAIL_PADDING_H_MIN 32

//play controllbar attributes
#define PLAY_CTRBAR_MIN_VERTICAL_H 320
#define PLAY_CTRBAR_TOP_PADDING_MIN_H 12
#define PLAY_CTRBAR_MIDDLE_PADDING_MIN_H 7
#define PLAY_CTRBAR_BOTTOM_PADDING_MIN_H 12
#define PLAY_CTRBAR_PROGRESS_BAR_VERTICAL_W 348
#define PLAY_CTRBAR_PROGRESS_BAR_HORIZONTAL_W 688
#define PLAYER_PROGRESSBAR_H 8
#define PLAYER_CIRCLE_W 26
#define PLAYER_CIRCLE_H 26
#define PLAYER_CIRCLE_PRESS_W 26
#define PLAYER_CIRCLE_PRESS_H 26
#define PLAY_CTRBAR_SMALL_BUTTON_W 126
#define PLAY_CTRBAR_LARGE_BUTTON_W 126
#define PLAY_CTRBAR_BUTTON_MARGIN 34
#define PLAY_CTRBAR_BUTTON_TEXT_MARGIN 14
#define PLAY_CTRBAR_PROGRESS_TEXT_MARGIN 14
#define PLAY_CTRBAR_PROGRESS_TEXT_W 40
#define PLAY_CTRBAR_BUTTON_VOLUME_START_X 18

//edc images
#define TITLE_BG "00_title_bg.#.png"
#define SMALL_BTN_CIRCLE "full_player_button_02.png"
#define LARGE_BTN_CIRCLE "full_player_button_01.png"
#define PREV_BTN "full_player_button_prev.png"
#define PLAY_BTN "full_player_button_play.png"
#define PAUSE_BTN "full_player_button_pause.png"
#define NEXT_BTN "full_player_button_next.png"
#define PROGRESS_BG "T02_progress_bg.png"
#define PROGRESS_BAR "T02_progress_bar.png"
#define PROGRESS_BAR_BUTTON "T02_progress_bar_button.png"
#define PROGRESS_BAR_BUTTON_PRESSED "T02_progress_bar_button_pressed.png"
#define LOCK_ICON "messages_state_ic_lock.png"
#define AUDIO_ICON "M01_icon_play.png"
#define FILE_BG "M01_file_bg.#.png"
#define FILE_BG_EFFECT "M01_file_bg_ef.#.png"

#define COLOR_BLACK 			0 0 0 255
#define COLOR_BLACK_TRANSPARENT 0 0 0 204
#define COLOR_WHITE				255 255 255 255
#define COLOR_WHITE_TRANSPARENT	255 255 255 128
#define COLOR_GRAY				245 245 242 255
#define COLOR_DARK_GRAY			213 213 212 255
#define COLOR_BLUE 				2 61 132 255
#define COLOR_DARK_BLUE			1 37 79 255
#define COLOR_BLUE_TRANSPARENT	179 197 218 255
#define COLOR_RED				224 33 33 255
#define BUBBLE_NAVI_TITLE_COLOR 61 185 204 255
#define COLOR_GRAY_TITLE_TEXT_EDC 128 128 128 255
#define COLOR_TIME_TEXT_EDC 102 102 102 255
#define COLOR_LOCK_BTN_EDC 179 179 179 255

//MACROS
#define PART_BG(pName) \
	part { \
		name: pName; \
		type: RECT; \
		mouse_events: 0; \
		scale: 1; \
		description { \
			state: "default" 0.0; \
			color: 0 0 0 0; \
		} \
	}

#define PART_RECT(pName, pR1x, pR1y, pR1xt, pR1yt, pR2x, pR2y, pR2xt, pR2yt) \
	part { \
		name: pName; \
		type: RECT; \
		mouse_events: 0; \
		scale: 1; \
		description { \
			state: "default" 0.0; \
			rel1 { relative: pR1x pR1y; to_x: pR1xt; to_y: pR1yt; } \
			rel2 { relative: pR2x pR2y; to_x: pR2xt; to_y: pR2yt; } \
			color: 0 0 0 0; \
		} \
	}

#define PART_PIXEL_RECT(pName, pX, pY, pAx, pAy, pRx, pRy, pRt) \
	part { \
		name: pName; \
		type: RECT; \
		mouse_events: 0; \
		scale: 1; \
		description { \
			state: "default" 0.0; \
			min: pX pY; \
			align: pAx pAy; \
			rel1 { relative: pRx pRy; to: pRt; } \
			rel2 { relative: pRx pRy; to: pRt; } \
			color: 0 0 0 0; \
		} \
	}

#define PART_PIXEL_RECT_EX(pName, pX, pY, pAx, pAy, pR1x, pR1y, pR2x, pR2y, pRt) \
	part { \
		name: pName; \
		type: RECT; \
		mouse_events: 0; \
		scale: 1; \
		description { \
			state: "default" 0.0; \
			min: pX pY; \
			max: pX pY; \
			align: pAx pAy; \
			rel1 { relative: pR1x pR1y; to: pRt; } \
			rel2 { relative: pR2x pR2y; to: pRt; } \
			color: 0 0 0 0; \
		} \
	}


#define PART_V_PIXEL_RECT(pName, pY, pAy, pRy, pRt) \
	part { \
		name: pName; \
		type: SPACER; \
		mouse_events: 0; \
		scale: 1; \
		description { \
			state: "default" 0.0; \
			min: 0 pY; \
			fixed: 0 1; \
			align: 0.5 pAy; \
			rel1 { relative: 0.0 pRy; to: pRt; } \
			rel2 { relative: 1.0 pRy; to: pRt; } \
		} \
	}

#define PART_H_PIXEL_RECT(pName, pX, pAx, pRx, pRt) \
	part { \
		name: pName; \
		type: SPACER; \
		mouse_events: 0; \
		scale: 1; \
		description { \
			state: "default" 0.0; \
			min: pX 0; \
			fixed: 1 0; \
			align: pAx 0.5; \
			rel1 { relative: pRx 0.0; to: pRt; } \
			rel2 { relative: pRx 1.0; to: pRt; } \
		} \
	}

#define PART_SWALLOW(pName, pTo) \
	part { \
		name: pName; \
		type: SWALLOW; \
		mouse_events: 1; \
		scale: 1; \
		description { \
			state: "default" 0.0; \
			fixed: 1 1; \
			rel1 { relative: 0.0 0.0; to: pTo; } \
			rel2 { relative: 1.0 1.0; to: pTo; } \
		} \
	}

#define PART_EVENT_RECT(pName, pTo) \
	part { \
		name: pName; \
		type: RECT; \
		mouse_events: 1; \
		repeat_events: 1; \
		scale: 1; \
		description { \
			state: "default" 0.0; \
			rel1 { relative: 0.0 0.0; to: pTo; } \
			rel2 { relative: 1.0 1.0; to: pTo; } \
			color: 0 0 0 0; \
			visible: 1; \
		} \
		description { \
			state: "disabled" 0.0; \
			inherit: "default" 0.0; \
			visible: 0; \
		} \
	}

#define PART_COLOR_RECT(pName, pCr, pCg, pCb, pCa, pTo) \
	part { \
		name: pName; \
		type: RECT; \
		mouse_events: 0; \
		scale: 1; \
		description { \
			state: "default" 0.0; \
			rel1 { relative: 0.0 0.0; to: pTo; } \
			rel2 { relative: 1.0 1.0; to: pTo; } \
			color: pCr pCg pCb pCa; \
		} \
	}

#define PART_SWALLOW_RECT_BAR(pName, pTo, pRx1, pRy1, pRx2, pRy2)\
	part{\
		name: pName;\
		type: SWALLOW;\
		mouse_events: 0; \
		scale: 1;\
		description{\
			state: "default" 0.0;\
			rel1 { relative: pRx1 pRy1; to: pTo;}\
			rel2 { relative: pRx2 pRy2; to: pTo;}\
		}\
	}

#define PART_TEXT(pName, pTo, pAx, pAy) \
	part { \
		name: pName; \
		type: TEXT; \
		mouse_events: 0; \
		scale: 1; \
		description { \
			state: "default" 0.0; \
			rel1 { relative: 0.0 0.0; to: pTo; } \
			rel2 { relative: 1.0 1.0; to: pTo; } \
			text { \
				size: 30; \
				align: pAx pAy; \
				ellipsis: 0.0; \
				text_class: "label"; \
			} \
			color: COLOR_BLACK; \
		} \
		description { \
			state: "pressed" 0.0; \
			inherit: "default" 0.0; \
			text.size: 30; \
			color: COLOR_BLACK; \
		} \
	}

#define PART_IMAGE(pName, pTo, pImage) \
	part { \
		name: pName; \
		type: IMAGE; \
		mouse_events: 0; \
		scale: 1; \
		description { \
			state: "default" 0.0; \
			rel1 { relative: 0.0 0.0; to: pTo; } \
			rel2 { relative: 1.0 1.0; to: pTo; } \
			image.normal: pImage; \
		} \
	}

#define PART_BORDER_IMAGE(pName, pTo, pImage, pL, pR, pT, pB) \
	part { \
		name: pName; \
		type: IMAGE; \
		mouse_events: 0; \
		scale: 1; \
		description { \
			state: "default" 0.0; \
			rel1 { relative: 0.0 0.0; to: pTo; } \
			rel2 { relative: 1.0 1.0; to: pTo; } \
			image.normal: pImage; \
			image.border: pL pR pT pB; \
		} \
	}

#define PART_TOP_LINE(pName, pTo) \
	part \
	{ \
		name: pName; \
		type: RECT; \
		mouse_events: 0; \
		description { \
			state: "default" 0.0; \
			min: 0 1; \
			fixed: 0 1; \
			visible: 1; \
			color: COLOR_BLUE; \
			rel1 { relative: 0.0 0.0; to: pTo; offset: 0 0; } \
			rel2 { relative: 1.0 0.0; to: pTo; offset: 0 0; } \
		} \
	} \

#define PART_LEFT_LINE(pName, pTo) \
	part \
	{ \
		name: pName; \
		type: RECT; \
		mouse_events: 0; \
		description { \
			state: "default" 0.0; \
			min: 1 0; \
			fixed: 1 0; \
			visible: 1; \
			color: COLOR_BLUE; \
			rel1 { relative: 0.0 0.0; to: pTo; offset: -1 0; } \
			rel2 { relative: 0.0 1.0; to: pTo; offset: -1 0; } \
		} \
	} \

#define PART_RIGHT_LINE(pName, pTo) \
	part \
	{ \
		name: pName; \
		type: RECT; \
		mouse_events: 0; \
		description { \
			state: "default" 0.0; \
			min: 1 0; \
			fixed: 1 0; \
			visible: 1; \
			color: COLOR_BLUE; \
			rel1 { relative: 1.0 0.0; to: pTo; offset: 1 0; } \
			rel2 { relative: 1.0 1.0; to: pTo; offset: 1 0; } \
		} \
	} \

#define PART_BOTTOM_LINE(pName, pTo) \
	part \
	{ \
		name: pName; \
		type: RECT; \
		mouse_events: 0; \
		description { \
			state: "default" 0.0; \
			min: 0 1; \
			fixed: 0 1; \
			visible: 1; \
			color: COLOR_BLUE; \
			rel1 { relative: 0.0 1.0; to: pTo; offset: 0 0; } \
			rel2 { relative: 1.0 1.0; to: pTo; offset: 0 0; } \
		} \
	} \

#define PART_BOTTOM_LINE_DOUBLE(pName, pTo) \
	part \
	{ \
		name: pName; \
		type: RECT; \
		mouse_events: 0; \
		scale: 1; \
		description { \
			state: "default" 0.0; \
			min: 0 1; \
			fixed: 0 1; \
			visible: 1; \
			color: 98 102 117 255; \
			rel1 { relative: 0.0 1.0; to: pTo; offset: 0 -1; } \
			rel2 { relative: 1.0 1.0; to: pTo; offset: 0 -1; } \
		} \
	} \
	part \
	{ \
		name: pName##"2"; \
		type: RECT; \
		mouse_events: 0; \
		scale: 1; \
		description { \
			state: "default" 0.0; \
			min: 0 1; \
			fixed: 0 1; \
			visible: 1; \
			color: 64 64 64 255; \
			rel1 { relative: 0.0 1.0; to: pTo; offset: 0 0; } \
			rel2 { relative: 1.0 1.0; to: pTo; offset: 0 0; } \
		} \
	} \

#define PART_ACCESS_RECT(pTo) \
	part \
	{ \
		name: "access"; \
		type: RECT; \
		repeat_events: 1; \
		description { \
			state: "default" 0.0; \
			fixed: 1 1; \
			color: 0 0 0 0; \
			rel1 { relative: 0.0 1.0; to: pTo; offset: 0 0; } \
			rel2 { relative: 1.0 1.0; to: pTo; offset: 0 0; } \
		} \
	} \

#define PART_PADDING_TOP(pName, pSize, pTo) \
	PART_V_PIXEL_RECT(pName, pSize, 0.0, 0.0, pTo)

#define PART_PADDING_BOTTOM(pName, pSize, pTo) \
	PART_V_PIXEL_RECT(pName, pSize, 1.0, 1.0, pTo)

#define PART_PADDING_LEFT(pName, pSize, pTo) \
	PART_H_PIXEL_RECT(pName, pSize, 0.0, 0.0, pTo)

#define PART_PADDING_RIGHT(pName, pSize, pTo) \
	PART_H_PIXEL_RECT(pName, pSize, 1.0, 1.0, pTo)

#define SCRIPT_CUSTOM_PART(pPart) \
	custom_state(PART:pPart, "default", 0.0); \
	set_state(PART:pPart, "custom", 0.0);

#define SCRIPT_SET_IMAGE(pPart, pVal) \
	set_state_val(PART:pPart, STATE_IMAGE, IMAGE:pVal);

#define SCRIPT_SET_VISIBLE(pPart, pVal) \
	set_state_val(PART:pPart, STATE_VISIBLE, pVal);

#define SCRIPT_SET_STATE_DISABLED(pPart) \
	set_state(PART:pPart, "disabled", 0.0);

#define SCRIPT_SET_STATE_DEFAULT(pPart) \
	set_state(PART:pPart, "default", 0.0);

#define SCRIPT_SET_STATE_SHOW(pPart) \
	set_state(PART:pPart, "show", 0.0);

#define SCRIPT_SET_STATE_HIDE(pPart) \
	set_state(PART:pPart, "hide", 0.0);

#define PROGRAM_SET_VISIBLE(pName, pSignal, pSource, pPart) \
	program { \
		name: pName; \
		signal: pSignal; \
		source: pSource; \
		script { \
			SCRIPT_SET_STATE_SHOW(pPart) \
		} \
	}

#define PROGRAM_SET_INVISIBLE(pName, pSignal, pSource, pPart) \
	program { \
		name: pName; \
		signal: pSignal; \
		source: pSource; \
		script { \
			SCRIPT_SET_STATE_HIDE(pPart) \
		} \
	}
