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

#include "model/models.hpp"

namespace Wt {
class WLineEdit;
class WPushButton;
}

namespace facts {

/** Root widget of application */
class FactsWidget : public Wt::WContainerWidget {
public:
    FactsWidget(Wt::WContainerWidget* p=0);

    /** Show the fact */
    void set_fact(const FactPtr& fact);

    /** Try to enter admin panel */
    void try_admin_enter();

private:
    Wt::WBorderLayout* layout_;
    Wt::WLineEdit* admin_password_;
    FactPtr shown_fact_;
    Wt::WPushButton* fact_id_;

    void setWidget(Wt::WWidget* widget,
                   Wt::WBorderLayout::Position position=Wt::WBorderLayout::Center);
    void enter_handler_();
    void set_random_fact_();
    void set_prev_fact_();
    void set_next_fact_();
    void id_clicked_handler_();
    void add_west_();
    void show_admin_widget_();
};

}

#endif

