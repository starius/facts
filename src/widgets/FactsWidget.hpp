/*
 * facts, random fact viewer
 * Copyright (C) 2011 Boris Nagaev
 *
 * facts is licensed under the GNU GPL Version 2.
 * Other versions of the GPL do not apply.
 * See the LICENSE file for terms of use.
 */

#ifndef FACTS_WIDGETS_FACTSWIDGET_HPP_
#define FACTS_WIDGETS_FACTSWIDGET_HPP_

#include <Wt/WContainerWidget>
#include <Wt/WBorderLayout>

#include "model/Fact.hpp"

namespace Wt {
class WLineEdit;
}

namespace facts {

/** Root widget of application */
class FactsWidget : public Wt::WContainerWidget {
public:
    FactsWidget(Wt::WContainerWidget* p=0);

private:
    Wt::WBorderLayout* layout_;
    Wt::WLineEdit* admin_password_;

    void setWidget(Wt::WWidget* widget,
                   Wt::WBorderLayout::Position position=Wt::WBorderLayout::Center);
    void enter_admin_handler_();
    void enter_handler_();
    void set_random_fact_();
    void set_fact_(FactPtr fact);
};

}

#endif

