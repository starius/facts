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
#include <Wt/WPushButton>
#include <Wt/WLineEdit>
#include <Wt/WText>
#include <Wt/WRegExpValidator>

#include "widgets/BanWidget.hpp"
#include "model/Ban.hpp"
#include "Application.hpp"

namespace facts {

typedef BanPtr Result;
typedef dbo::Query<Result> Q;
typedef dbo::QueryModel<Result> BaseQM;

const int BANNED_COLUMN = 0;
const int IP_COLUMN = 1;
const int WHEN_ADDED_COLUMN = 2;

const std::string IP_RE = "((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}"
                          "(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)";

class BansModel : public BaseQM {
public:
    BansModel(const Q& query, Wt::WObject *parent=0) :
        BaseQM(parent) {
        setQuery(query);
        addColumn("banned", tr("facts.admin.banned"), Wt::ItemIsEditable | Wt::ItemIsUserCheckable);
        addColumn("ip", tr("facts.admin.ip"));
        addColumn("when_added", tr("facts.fact.when_added"));
    }

    boost::any data(const Wt::WModelIndex& index,
                    int role=Wt::DisplayRole) const {
        dbo::Transaction t(fApp->session());
        const BanPtr& o = resultRow(index.row());
        if (index.column() == BANNED_COLUMN) {
            if (role == Wt::DisplayRole) {
                return "";
            } else if (role == Wt::CheckStateRole) {
                return o->banned();
            }
        }
        return BaseQM::data(index, role);
    }

    bool setData(const Wt::WModelIndex& index, const boost::any& value,
                 int role=Wt::EditRole) {
        if (role == Wt::CheckStateRole && value.type() == typeid(bool)) {
            dbo::Transaction t(fApp->session());
            const BanPtr& o = resultRow(index.row());
            o.modify()->set_banned(boost::any_cast<bool>(value));
            t.commit();
            dataChanged().emit(index, index);
            return true;
        }
        return BaseQM::setData(index, value, Wt::EditRole);
    }

    static Wt::WString tr(const char* key) {
        return Wt::WString::tr(key);
    }
};

class BansView : public Wt::WTableView {
public:
    BansView(BansModel* model, Wt::WContainerWidget* p=0):
        Wt::WTableView(p) {
        setModel(model);
        resize(500, 250);
        setColumnWidth(BANNED_COLUMN, 80);
        setColumnWidth(IP_COLUMN, 150);
        setColumnWidth(WHEN_ADDED_COLUMN, 75);
        setRowHeaderCount(1);
        setAlternatingRowColors(true);
    }
};

BanWidget::BanWidget(Wt::WContainerWidget* p):
    Wt::WContainerWidget(p) {
    dbo::Transaction t(fApp->session());
    Q query = fApp->session().find<Ban>();
    BansModel* model = new BansModel(query, this);
    view_ = new BansView(model, this);
    ip_ = new Wt::WLineEdit(fApp->ip_from_path(), this);
    ip_->setEmptyText(tr("facts.admin.Enter_ip"));
    ip_->setValidator(new Wt::WRegExpValidator(IP_RE));
    ip_->validator()->setMandatory(true);
    Wt::WPushButton* add = new Wt::WPushButton(tr("facts.admin.Add"), this);
    add->clicked().connect(this, &BanWidget::add_handler_);
    error_ = new Wt::WText(this);
    t.commit();
}

void BanWidget::add_handler_() {
    dbo::Transaction t(fApp->session());
    Wt::WValidator::State V = Wt::WValidator::Valid;
    if (ip_->validate() != V) {
        error_->setText(tr("facts.admin.Wrong_ip"));
        return;
    }
    std::string ip = ip_->text().toUTF8();
    try {
        BanPtr test = fApp->session().load<Ban>(ip);
        error_->setText(tr("facts.admin.Duplicate_ip"));
        return;
    } catch (...)
    { }
    fApp->session().add(new Ban(ip));
    fApp->session().flush();
    static_cast<BansModel*>(view_->model())->reload();
    t.commit();
    error_->setText("");
}

}

