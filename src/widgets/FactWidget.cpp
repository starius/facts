/*
 * facts, random fact viewer
 * Copyright (C) 2011 Boris Nagaev
 *
 * facts is licensed under the GNU GPL Version 2.
 * Other versions of the GPL do not apply.
 * See the LICENSE file for terms of use.
 */

#include <boost/lexical_cast.hpp>

#include <Wt/WEnvironment>
#include <Wt/WText>
#include <Wt/WBreak>
#include <Wt/WImage>
#include <Wt/Dbo/Transaction>
namespace dbo = Wt::Dbo;

#include "widgets/FactWidget.hpp"
#include "Application.hpp"

namespace facts {

FactWidget::FactWidget(FactPtr fact, Wt::WContainerWidget* p):
    Wt::WContainerWidget(p),
    fact_(fact) {
    resize(Wt::WLength(70, Wt::WLength::Percentage), Wt::WLength());
    dbo::Transaction t(fApp->session());
    new Wt::WText(fact_->text(), this);
    new Wt::WBreak(this);
    Wt::WImage* diff_minus = new Wt::WImage("img/down-arrow.png", this);
    score_ = new Wt::WText(this);
    Wt::WImage* diff_plus = new Wt::WImage("img/up-arrow.png", this);
    set_score_();
    diff_minus->setVerticalAlignment(Wt::AlignMiddle);
    score_->setVerticalAlignment(Wt::AlignMiddle);
    diff_plus->setVerticalAlignment(Wt::AlignMiddle);
    diff_minus->clicked().connect(boost::bind(&FactWidget::vote_, this, -1));
    diff_plus->clicked().connect(boost::bind(&FactWidget::vote_, this, +1));
    diff_minus->decorationStyle().setCursor(Wt::PointingHandCursor);
    diff_plus->decorationStyle().setCursor(Wt::PointingHandCursor);
    t.commit();
}

void FactWidget::set_score_() {
    dbo::Transaction t(fApp->session());
    score_->setText(boost::lexical_cast<std::string>(fact_->score()));
    t.commit();
}

void FactWidget::vote_(short diff) {
    dbo::Transaction t(fApp->session());
    fact_.reread();
    VoteId vote_id;
    vote_id.fact = fact_,
            vote_id.ip = fApp->environment().clientAddress();
    try {
        fApp->session().load<Vote>(vote_id);
        score_->setText(tr("facts.fact.Already_voted"));
    } catch (dbo::ObjectNotFoundException) {
        Vote* vote = new Vote(fact_, vote_id.ip, diff);
        fApp->session().add(vote);
        set_score_();
    }
    t.commit();
}

}

