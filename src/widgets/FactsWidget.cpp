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
#include <Wt/WTemplate>

#include "widgets/FactsWidget.hpp"
#include "widgets/FactWidget.hpp"
#include "widgets/AdminWidget.hpp"
#include "model/Fact.hpp"
#include "Session.hpp"
#include "Application.hpp"
#include "config.hpp"
#include "utils.hpp"

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
    a_img("img/logo.png", fApp->admin_path(), logo_c);
    logo_c->setContentAlignment(Wt::AlignCenter);
    layout_->addWidget(logo_c, Wt::WBorderLayout::North);
    add_west_();
}

void FactsWidget::setWidget(Wt::WWidget* widget, Wt::WBorderLayout::Position position) {
    WWidget* old = layout_->widgetAt(position);
    if (old) {
        delete old;
        layout_->removeWidget(old);
    }
    layout_->addWidget(widget, position);
}

void FactsWidget::try_admin_enter() {
    if (fApp->admin()) {
        show_admin_widget_();
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
        show_admin_widget_();
    } else {
        setWidget(new Wt::WText(tr("facts.admin.Wrong_admin_password")));
    }
}

void FactsWidget::set_prev_fact() {
    dbo::Transaction t(fApp->session());
    try {
        FactPtr fact = fApp->session().load<Fact>(shown_fact_.id() - 1);
        set_fact(fact);
    } catch (dbo::ObjectNotFoundException)
    { }
    t.commit();
}

void FactsWidget::set_next_fact() {
    dbo::Transaction t(fApp->session());
    try {
        FactPtr fact = fApp->session().load<Fact>(shown_fact_.id() + 1);
        set_fact(fact);
    } catch (dbo::ObjectNotFoundException)
    { }
    t.commit();
}

void FactsWidget::set_random_fact() {
    dbo::Transaction t(fApp->session());
    int facts_number = fApp->session().query<int>("select count(1) from facts_fact")
                       .where("id != ?").bind(shown_fact_.id());
    int offset = Wt::WRandom::get() % facts_number;
    FactPtr fact = fApp->session().find<Fact>().where("id != ?").bind(shown_fact_.id())
                   .offset(offset).limit(1).resultValue();
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

void FactsWidget::set_fact(const FactPtr& fact) {
    dbo::Transaction t(fApp->session());
    shown_fact_ = fact;
    fApp->setInternalPath(fApp->fact_path(fact));
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
    a_img("img/update.png", fApp->random_fact_path(), west)->setInline(false);
    a_img("img/left-arrow.png", fApp->prev_fact_path(), west);
    fact_id_ = new Wt::WPushButton(west);
    fact_id_->clicked().connect(this, &FactsWidget::id_clicked_handler_);
    fact_id_->setVerticalAlignment(Wt::AlignMiddle);
    a_img("img/right-arrow.png", fApp->next_fact_path(), west);
}

void FactsWidget::show_admin_widget_() {
    fApp->setInternalPath(fApp->admin_path());
    setWidget(new AdminWidget());
}

}

