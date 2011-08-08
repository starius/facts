/*
 * facts, random fact viewer
 * Copyright (C) 2011 Boris Nagaev
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
#include <Wt/WTemplate>

#include "widgets/FactsWidget.hpp"
#include "widgets/FactWidget.hpp"
#include "widgets/AdminWidget.hpp"
#include "model/Fact.hpp"
#include "Session.hpp"
#include "Application.hpp"
#include "config.hpp"

namespace facts {

class AutoSelectedEdit : public Wt::WLineEdit {
public:
    AutoSelectedEdit(const Wt::WString& content,
                     Wt::WContainerWidget* p=0, int size=72):
        Wt::WLineEdit(content, p) {
        setTextSize(size);
        clicked().connect(
            "function(object, event) {"
            "$(object).select();"
            "}"
        );
    }
};

FactsWidget::FactsWidget(Wt::WContainerWidget* p):
    Wt::WContainerWidget(p) {
    layout_ = new Wt::WBorderLayout();
    setLayout(layout_, Wt::AlignTop | Wt::AlignJustify);
    Wt::WContainerWidget* logo_c = new Wt::WContainerWidget();
    Wt::WImage* logo = new Wt::WImage("img/logo.png", logo_c);
    logo->clicked().connect(this, &FactsWidget::enter_admin_handler_);
    logo_c->setContentAlignment(Wt::AlignCenter);
    layout_->addWidget(logo_c, Wt::WBorderLayout::North);
    add_west_();
    set_random_fact_();
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
    if (fApp->admin()) {
        setWidget(new AdminWidget());
    } else {
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
}

void FactsWidget::enter_handler_() {
    if (admin_password_->text() == ADMIN_PASSWORD) {
        fApp->set_admin();
        setWidget(new AdminWidget());
    } else {
        setWidget(new Wt::WText(tr("facts.admin.Wrong_admin_password")));
    }
}

void FactsWidget::set_prev_fact_() {
    dbo::Transaction t(fApp->session());
    try {
        FactPtr fact = fApp->session().load<Fact>(shown_fact_.id() - 1);
        set_fact(fact);
    } catch (dbo::ObjectNotFoundException)
    { }
    t.commit();
}

void FactsWidget::set_next_fact_() {
    dbo::Transaction t(fApp->session());
    try {
        FactPtr fact = fApp->session().load<Fact>(shown_fact_.id() + 1);
        set_fact(fact);
    } catch (dbo::ObjectNotFoundException)
    { }
    t.commit();
}

void FactsWidget::set_random_fact_() {
    dbo::Transaction t(fApp->session());
    int facts_number = fApp->session().query<int>("select count(1) from facts_fact");
    FactPtr fact;
    while (!fact || (fact == shown_fact_ && facts_number > 1)) {
        int offset = Wt::WRandom::get() % facts_number;
        fact = fApp->session().find<Fact>().offset(offset).limit(1).resultValue();
    }
    set_fact(fact);
    t.commit();
}

void FactsWidget::id_clicked_handler_() {
    Wt::WTemplate* refs = new Wt::WTemplate(tr("facts.fact.references_template"));
    setWidget(refs);
    std::string url = fApp->makeAbsoluteUrl(fApp->bookmarkUrl(fApp->fact_path(shown_fact_)));
    AutoSelectedEdit* simple = new AutoSelectedEdit(url);
    AutoSelectedEdit* forum = new AutoSelectedEdit(tr("facts.fact.forum").arg(url));
    AutoSelectedEdit* html = new AutoSelectedEdit(tr("facts.fact.html").arg(url));
    refs->bindWidget("simple", simple);
    refs->bindWidget("forum", forum);
    refs->bindWidget("html", html);
}

void FactsWidget::set_fact(FactPtr fact) {
    dbo::Transaction t(fApp->session());
    shown_fact_ = fact;
    setWidget(new FactWidget(fact));
    fApp->setTitle(fact->text());
    fact_id_->setText(boost::lexical_cast<std::string>(fact.id()));
    t.commit();
}

void FactsWidget::add_west_() {
    Wt::WContainerWidget* west = new Wt::WContainerWidget();
    setWidget(west, Wt::WBorderLayout::West);
    west->setMinimumSize(175, Wt::WLength());
    west->setContentAlignment(Wt::AlignCenter);
    Wt::WImage* update = new Wt::WImage("img/update.png", west);
    update->clicked().connect(this, &FactsWidget::set_random_fact_);
    update->decorationStyle().setCursor(Wt::PointingHandCursor);
    update->setInline(false);
    Wt::WImage* prev = new Wt::WImage("img/left-arrow.png", west);
    prev->clicked().connect(this, &FactsWidget::set_prev_fact_);
    prev->decorationStyle().setCursor(Wt::PointingHandCursor);
    prev->setVerticalAlignment(Wt::AlignMiddle);
    fact_id_ = new Wt::WPushButton(west);
    fact_id_->clicked().connect(this, &FactsWidget::id_clicked_handler_);
    fact_id_->setVerticalAlignment(Wt::AlignMiddle);
    Wt::WImage* next = new Wt::WImage("img/right-arrow.png", west);
    next->clicked().connect(this, &FactsWidget::set_next_fact_);
    next->decorationStyle().setCursor(Wt::PointingHandCursor);
    next->setVerticalAlignment(Wt::AlignMiddle);
}

}

