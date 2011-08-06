/*
 * facts, random fact viewer
 * Copyright (C) 2010 Boris Nagaev
 *
 * facts is licensed under the GNU GPL Version 2.
 * Other versions of the GPL do not apply.
 * See the LICENSE file for terms of use.
 */

#include <Wt/Dbo/Transaction>
#include <Wt/WRandom>
#include <Wt/WPushButton>
#include <Wt/WLineEdit>
#include <Wt/WText>
#include <Wt/WEnvironment>
#include <Wt/WImage>

#include "widgets/FactsWidget.hpp"
#include "widgets/FactWidget.hpp"
#include "widgets/AdminWidget.hpp"
#include "model/Fact.hpp"
#include "Session.hpp"
#include "Application.hpp"
#include "config.hpp"

namespace facts {

FactsWidget::FactsWidget(Wt::WContainerWidget* p):
    Wt::WContainerWidget(p) {
    layout_ = new Wt::WBorderLayout();
    setLayout(layout_, Wt::AlignTop | Wt::AlignCenter);
    Wt::WContainerWidget* logo_c = new Wt::WContainerWidget();
    Wt::WImage* logo = new Wt::WImage("img/logo.png", logo_c);
    logo->clicked().connect(this, &FactsWidget::enter_admin_handler_);
    logo_c->setContentAlignment(Wt::AlignCenter);
    layout_->addWidget(logo_c, Wt::WBorderLayout::North);
    set_random_fact_();
    Wt::WImage* update = new Wt::WImage("img/update.png");
    update->clicked().connect(this, &FactsWidget::set_random_fact_);
    update->decorationStyle().setCursor(Wt::PointingHandCursor);
    setWidget(update, Wt::WBorderLayout::West);
}

void FactsWidget::setWidget(Wt::WWidget* widget, Wt::WBorderLayout::Position position) {
    WWidget* old = layout_->widgetAt(position);
    if (old) {
        delete old;
        layout_->removeWidget(old);
    }
    layout_->addWidget(widget, position);
}

void FactsWidget::enter_admin_handler_() {
    Wt::WContainerWidget* p = new Wt::WContainerWidget();
    admin_password_ = new Wt::WLineEdit(p);
    admin_password_->setEchoMode(Wt::WLineEdit::Password);
    admin_password_->setEmptyText(tr("facts.admin.Enter_admin_password"));
    admin_password_->enterPressed().connect(this, &FactsWidget::enter_handler_);
    if (!fApp->environment().ajax()) {
        Wt::WPushButton* enter = new Wt::WPushButton(tr("facts.admin.Enter_admin"), p);
        enter->clicked().connect(this, &FactsWidget::enter_handler_);
    }
    setWidget(p);
}

void FactsWidget::enter_handler_() {
    if (admin_password_->text() == ADMIN_PASSWORD) {
        setWidget(new AdminWidget());
    } else {
        setWidget(new Wt::WText(tr("facts.admin.Wrong_admin_password")));
    }
}

void FactsWidget::set_random_fact_() {
    dbo::Transaction t(fApp->session());
    int facts_number = fApp->session().query<int>("select count(1) from facts_fact");
    int i = Wt::WRandom::get() % facts_number;
    FactPtr fact = fApp->session().find<Fact>().offset(i).limit(1).resultValue();
    set_fact_(fact);
    t.commit();
}

void FactsWidget::set_fact_(FactPtr fact) {
    dbo::Transaction t(fApp->session());
    setWidget(new FactWidget(fact));
    t.commit();
}

}

