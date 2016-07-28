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

#ifndef Box_h_
#define Box_h_

#include "View.h"
#include <Elementary.h>

namespace Msg
{
    /**
     * @brief Wraps all basic Box operations.
     */
    class Box
        : public View
    {
        public:
            Box(Evas_Object *parent);
            virtual ~Box();

            /**
             * @brief Set the horizontal orientation of box.
             * @param[in] val if true box's orientation becomes horizontal otherwise it becomes vertical.
             */
            inline void setHorizontal(bool val);

            /**
             * @brief Manages homogeneous mode of box.
             * @param[in] val if true homogeneous mode is on, otherwise it's off.
             */
            inline void setHomogeneous(bool val);

            /**
             * @brief Add an object to the beginning of the pack list.
             * @param[in] obj an object to be added.
             */
            inline void packStart(Evas_Object *obj);

            /**
             * @brief Add an object at the end of the pack list.
             * @param[in] obj an object to be added.
             */
            inline void packEnd(Evas_Object *obj);

            /**
             * @brief Adds an object to the box before the indicated object.
             * @param[in] subobj an object to be added.
             * @param[in] before the object before which to add it.
             */
            inline void packBefore(Evas_Object *subobj, Evas_Object *before);

            /**
             * @brief Adds an object to the box after the indicated object.
             * @param[in] subobj an object to be added.
             * @param[in] after the object after which to add it.
             */
            inline void packAfter(Evas_Object *subobj, Evas_Object *after);

            /**
             * @brief Remove the object given by @c subobj from the box without deleting it.
             * @param[in] subobj an object to be removed.
             */
            inline void unpack(Evas_Object *subobj);

            /**
             * @brief Remove all items from the box, without deleting them.
             */
            inline void unpackAll();

            /**
             * @brief Clear the box of all children, children will be deleted.
             */
            inline void clear();

            /**
             * @brief Force the box to recalculate its children packing.
             */
            inline void recalculate();
    };

    inline void Box::setHorizontal(bool val)
    {
        elm_box_horizontal_set(getEo(), val);
    }

    inline void Box::setHomogeneous(bool val)
    {
        elm_box_homogeneous_set(getEo(), val);
    }

    inline void Box::packStart(Evas_Object *obj)
    {
        elm_box_pack_start(getEo(), obj);
    }

    inline void Box::packEnd(Evas_Object *obj)
    {
        elm_box_pack_end(getEo(), obj);
    }

    inline void Box::packBefore(Evas_Object *subobj, Evas_Object *before)
    {
        elm_box_pack_before(getEo(), subobj, before);
    }

    inline void Box::packAfter(Evas_Object *subobj, Evas_Object *after)
    {
        elm_box_pack_after(getEo(), subobj, after);
    }

    inline void Box::unpack(Evas_Object *subobj)
    {
        elm_box_unpack(getEo(), subobj);
    }

    inline void Box::unpackAll()
    {
        elm_box_unpack_all(getEo());
    }

    inline void Box::clear()
    {
        elm_box_clear(getEo());
    }

    inline void Box::recalculate()
    {
        elm_box_recalculate(getEo());
    }
}

#endif /* Box_h_ */
