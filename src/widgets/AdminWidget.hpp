/*
 * facts, random fact viewer
 * Copyright (C) 2011 Boris Nagaev
 *
 * facts is licensed under the GNU GPL Version 2.
 * Other versions of the GPL do not apply.
 * See the LICENSE file for terms of use.
 */

#ifndef FACTS_WIDGETS_ADMIN_WIDGET_HPP_
#define FACTS_WIDGETS_ADMIN_WIDGET_HPP_

#include <Wt/WContainerWidget>

namespace facts {
class FactListModel;

/** Widget to make admin changes of site.
Does not check admin permissions of user! Creator of this widget should check
them before creating this widget! */
class AdminWidget : public Wt::WContainerWidget {
public:
    AdminWidget(Wt::WContainerWidget* p=0);

private:
    FactListModel* model_;

    void save_handler_();
    void add_handler_();
};

}

#endif

