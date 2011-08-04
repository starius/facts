/*
 * facts, random fact viewer
 * Copyright (C) 2010 Boris Nagaev
 *
 * facts is licensed under the GNU GPL Version 2.
 * Other versions of the GPL do not apply.
 * See the LICENSE file for terms of use.
 */

#include <Wt/WText>
#include <Wt/Dbo/Transaction>
namespace dbo = Wt::Dbo;

#include "widgets/FactWidget.hpp"
#include "Application.hpp"

namespace facts {

FactWidget::FactWidget(FactPtr fact):
    fact_(fact) {
    dbo::Transaction t(fApp->session());
    new Wt::WText(fact_->text(), this);
    t.commit();
}

}


