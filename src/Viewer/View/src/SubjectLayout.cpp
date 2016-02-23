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

#include "SubjectLayout.h"
#include "Resource.h"
#include <sstream>

using namespace Msg;

namespace
{
    const char *groupName = "subject/pageindex";
    const char *partSubject = "subject/text";
    const char *partPageText = "page/text";
}

SubjectLayout::SubjectLayout(Evas_Object *parent)
    : m_NumberOfPages(0)
{
    setEo(createLayout(parent));
}

SubjectLayout::~SubjectLayout()
{

}

Evas_Object *SubjectLayout::createLayout(Evas_Object *parent)
{
    Evas_Object *layout = addLayout(parent, VIEWER_SUBJECT_LAYOUT_EDJ_PATH, groupName);
    return layout;
}

void SubjectLayout::setSubjectText(const std::string &subject)
{
    std::ostringstream subjectText;
    subjectText << "(";
    if(subject.empty())
        subjectText << (std::string)msg("IDS_MSGF_BODY_NO_SUBJECT");
    else
        subjectText << subject;
    subjectText << ")";
    setText(subjectText.str(), partSubject);
}

void SubjectLayout::setNumberOfPages(int numberOfPages)
{
    m_NumberOfPages = numberOfPages;
}

void SubjectLayout::setPageIndex(int pageIndex)
{
    std::ostringstream pageText;
    pageText << "(" << pageIndex << "/" << m_NumberOfPages << ")";
    setText(pageText.str(), partPageText);
}
