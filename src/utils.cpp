/*
 * facts, random fact viewer
 * Copyright (C) 2011 Boris Nagaev
 *
 * facts is licensed under the GNU GPL Version 2.
 * Other versions of the GPL do not apply.
 * See the LICENSE file for terms of use.
 */

#include <Wt/WImage>
#include <Wt/WAnchor>
#include <Wt/WEnvironment>

#include "utils.hpp"
#include "model/Fact.hpp"
#include "Application.hpp"

namespace facts {

Wt::WInteractWidget* a_img(const std::string& src, const std::string& href,
                           Wt::WContainerWidget* parent) {
    Wt::WInteractWidget* result;
    Wt::WImage* img = new Wt::WImage(src);
    img->setVerticalAlignment(Wt::AlignMiddle);
    if (fApp->environment().ajax()) {
        img->clicked().connect(boost::bind(&Wt::WApplication::setInternalPath,
                                           fApp, href, /*emit*/ true));
        img->decorationStyle().setCursor(Wt::PointingHandCursor);
        result = img;
    } else {
        Wt::WAnchor* a = new Wt::WAnchor();
        a->setRefInternalPath(href);
        a->setImage(img);
        result = a;
    }
    if (parent) {
        parent->addWidget(result);
    }
    return result;
}

}

