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
#include <Wt/WTemplate>
#include <Wt/WTable>
#include <Wt/WBreak>
#include <Wt/WImage>
#include <Wt/Dbo/Transaction>
namespace dbo = Wt::Dbo;

#include "widgets/FactWidget.hpp"
#include "model/Fact.hpp"
#include "widgets/CommentsWidget.hpp"
#include "Application.hpp"

namespace facts {

class ExternalCounters : public Wt::WTable {
public:
    ExternalCounters(FactWidget* p=0):
        Wt::WTable(p) {
        std::string url = fApp->makeAbsoluteUrl(fApp->bookmarkUrl(fApp->fact_path(p->fact())));
        const std::string& vk_div = elementAt(0, 0)->id();
        const Wt::WString& text = p->fact()->text();
        int fact_id = p->fact().id();
        boost::format vk("VK.Widgets.Like('%s', {type: 'vertical', pageUrl: '%s', pageTitle: '%s'}, %i);");
        doJavaScript(str(vk % vk_div % url % text % fact_id));
        Wt::WTemplate* twit = new Wt::WTemplate();
        twit->setTemplateText(tr("facts.counters.twit_template"), Wt::XHTMLUnsafeText);
        elementAt(0, 1)->addWidget(twit);
    }
};

FactWidget::FactWidget(const FactPtr& fact, Wt::WContainerWidget* p):
    Wt::WContainerWidget(p),
    fact_(fact) {
    resize(Wt::WLength(70, Wt::WLength::Percentage), Wt::WLength());
    dbo::Transaction t(fApp->session());
    Wt::WText* text = new Wt::WText(fact_->text(), this);
    text->setStyleClass("facts-text");
    new Wt::WBreak(this);
    new ExternalCounters(this);
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
    new Wt::WBreak(this);
    new Wt::WBreak(this);
    new CommentsWidget(fact_, this);
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
    VoteId vote_id(fact_, fApp->environment().clientAddress());
    try {
        fApp->session().load<Vote>(vote_id);
        score_->setText(tr("facts.fact.Already_voted"));
        return;
    } catch (dbo::ObjectNotFoundException)
    { }
    if (fApp->is_banned()) {
        score_->setText(tr("facts.common.BannedIp"));
        return;
    }
    fApp->session().add(new Vote(vote_id, diff));
    set_score_();
    t.commit();
}

}

