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

#include "widgets/FactsWidget.hpp"
#include "widgets/FactWidget.hpp"
#include "model/Fact.hpp"
#include "Session.hpp"
#include "Application.hpp"

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
}

}

