/*
 * facts, random fact viewer
 * Copyright (C) 2011 Boris Nagaev
 *
 * facts is licensed under the GNU GPL Version 2.
 * Other versions of the GPL do not apply.
 * See the LICENSE file for terms of use.
 */

#include <Wt/WTableView>
#include <Wt/Dbo/Transaction>
#include <Wt/Dbo/Query>
#include <Wt/Dbo/QueryModel>
#include <Wt/WString>
#include <Wt/WPushButton>

#include "widgets/AdminWidget.hpp"
#include "model/Fact.hpp"
#include "Application.hpp"

namespace facts {

typedef FactPtr Result;
typedef dbo::Query<Result> Q;
typedef dbo::QueryModel<Result> BaseQM;

const int n_column = 0;
const int text_column = 1;
const int when_added_column = 2;

class FactListModel : public BaseQM {
public:
    FactListModel(const Q& query, Wt::WObject *parent=0) :
        BaseQM(parent) {
        setQuery(query);
        addColumn("id", tr("facts.fact.id"));
        addColumn("text", tr("facts.fact.text"), Wt::ItemIsEditable);
        addColumn("when_added", tr("facts.fact.when_added"));
    }

    boost::any data(const Wt::WModelIndex& index,
                    int role=Wt::DisplayRole) const {
        dbo::Transaction t(fApp->session());
        if (role == Wt::InternalPathRole && index.column() == n_column) {
            FactPtr o = resultRow(index.row());
            return fApp->fact_path(o);
        }
        return BaseQM::data(index, role);
    }

    static Wt::WString tr(const char* key) {
        return Wt::WString::tr(key);
    }
};

class FactListView : public Wt::WTableView {
public:
    FactListView(FactListModel* model, Wt::WContainerWidget* p=0):
        Wt::WTableView(p) {
        setModel(model);
        resize(770, 450);
        setColumnWidth(n_column, 40);
        setColumnWidth(text_column, 570);
        setColumnWidth(when_added_column, 75);
        setRowHeaderCount(1);
    }
};

AdminWidget::AdminWidget(Wt::WContainerWidget* p):
    Wt::WContainerWidget(p) {
    dbo::Transaction t(fApp->session());
    Wt::WPushButton* save = new Wt::WPushButton(tr("facts.admin.Save"), this);
    save->clicked().connect(this, &AdminWidget::save_handler_);
    Wt::WPushButton* add = new Wt::WPushButton(tr("facts.admin.Add"), this);
    add->clicked().connect(this, &AdminWidget::add_handler_);
    Q query = fApp->session().find<Fact>();
    FactListModel* model = new FactListModel(query, this);
    view_ = new FactListView(model, this);
    t.commit();
}

void AdminWidget::save_handler_() {
    view_->closeEditors();
    dbo::Transaction t(fApp->session());
    fApp->session().flush();
    t.commit();
}

void AdminWidget::add_handler_() {
    view_->closeEditors();
    dbo::Transaction t(fApp->session());
    fApp->session().add(new Fact(true));
    fApp->session().flush();
    static_cast<FactListModel*>(view_->model())->reload();
    t.commit();
}

}

