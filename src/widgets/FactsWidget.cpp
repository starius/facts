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
    Session& s = fApp->session();
    dbo::Transaction t(s);
    int facts_number = s.query<int>("select count(1) from facts_fact");
    int i = Wt::WRandom::get() % facts_number;
    FactPtr fact = s.find<Fact>().offset(i).limit(1).resultValue();
    new FactWidget(fact, this);
    t.commit();
    Wt::WPushButton* admin = new Wt::WPushButton(tr("facts.admin.Enter_admin"), this);
    admin->clicked().connect(this, &FactsWidget::enter_admin_handler_);
}

void FactsWidget::enter_admin_handler_() {
    static_cast<Wt::WPushButton*>(sender())->hide();
    Wt::WLineEdit* admin_password = new Wt::WLineEdit(this);
    admin_password->setEchoMode(Wt::WLineEdit::Password);
    admin_password->setEmptyText(tr("facts.admin.Enter_admin_password"));
    admin_password->enterPressed().connect(this, &FactsWidget::enter_handler_);
}

void FactsWidget::enter_handler_() {
    Wt::WLineEdit* admin_password = static_cast<Wt::WLineEdit*>(sender());
    if (admin_password->text() == ADMIN_PASSWORD) {
        clear();
        new AdminWidget(this);
    } else {
        admin_password->hide();
        new Wt::WText(tr("facts.admin.Wrong_admin_password"), this);
    }
}

}

