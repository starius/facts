/*
 * facts, random fact viewer
 * Copyright (C) 2011 Boris Nagaev
 *
 * facts is licensed under the GNU GPL Version 2.
 * Other versions of the GPL do not apply.
 * See the LICENSE file for terms of use.
 */

#ifndef FACTS_WIDGETS_BAN_WIDGET_HPP_
#define FACTS_WIDGETS_BAN_WIDGET_HPP_

#include <Wt/WContainerWidget>

namespace Wt {
class WLineEdit;
class WText;
}

namespace facts {
class BansView;

/** Widget to make admin changes of site. */
class BanWidget : public Wt::WContainerWidget {
public:
    BanWidget(Wt::WContainerWidget* p=0);

private:
    BansView* view_;
    Wt::WLineEdit* ip_;
    Wt::WText* error_;

    void add_handler_();
};

}

#endif

