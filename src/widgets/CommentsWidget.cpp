/*
 * acts, random fact viewer
 * Copyright (C) 2011 Boris Nagaev
 *
 * facts is licensed under the GNU GPL Version 2.
 * Other versions of the GPL do not apply.
 * See the LICENSE file for terms of use.
 */

#include <Wt/WEnvironment>
#include <Wt/WContainerWidget>
#include <Wt/WTableView>
#include <Wt/Dbo/Transaction>
#include <Wt/Dbo/Query>
#include <Wt/Dbo/QueryModel>
#include <Wt/WText>
#include <Wt/WTextEdit>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WLengthValidator>
#include <Wt/WRegExpValidator>

#include "widgets/CommentsWidget.hpp"
#include "model/Fact.hpp"
#include "model/Comment.hpp"
#include "Application.hpp"

namespace facts {

typedef CommentPtr Result;
typedef dbo::Query<Result> Q;
typedef dbo::QueryModel<Result> BaseQM;

const int INPUT_SIZE = 25;
const int TEXT_COLUMNS = 80;
const int ROW_HEIGHT = 140;
const int MIN_INPUT_SIZE = 2;
const Wt::WString EMAIL_PATTERN = "[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,4}";
const int MAX_TEXT_SIZE = 2000;

class CommentsModel : public BaseQM {
public:
    CommentsModel(const Q& query, Wt::WObject *parent=0) :
        BaseQM(parent),
        deleted_column(fApp->admin() ? 0 : -1),
        index_column(fApp->admin() ? 1 : 0),
        text_column(fApp->admin() ? 2 : 1) {
        setQuery(query);
        if (deleted_column != -1) {
            addColumn("deleted", "", Wt::ItemIsEditable | Wt::ItemIsUserCheckable);
        }
        addColumn("comment_index", "");
        addColumn("text", "", Wt::ItemIsXHTMLText);
    }

    boost::any data(const Wt::WModelIndex& index,
                    int role=Wt::DisplayRole) const {
        dbo::Transaction t(fApp->session());
        const CommentPtr& o = resultRow(index.row());
        if (index.column() == deleted_column) {
            if (role == Wt::DisplayRole) {
                return "";
            } else if (role == Wt::CheckStateRole) {
                return o->deleted();
            }
        } else if (index.column() == index_column) {
            if (role == Wt::InternalPathRole) {
                return fApp->comment_path(o);
            } else if (role == Wt::DisplayRole) {
                return tr("facts.common.id_format").arg(o.id().index);
            }
        } else if (index.column() == text_column && role == Wt::DisplayRole) {
            if (o->deleted()) {
                return tr("facts.comment.deleted");
            } else {
                return tr("facts.comment.format").arg(o->username()).arg(o->email())
                       .arg(o->when_added().toString()).arg(o->text());
            }
        }
        return BaseQM::data(index, role);
    }

    bool setData(const Wt::WModelIndex& index, const boost::any& value,
                 int role=Wt::EditRole) {
        if (role == Wt::CheckStateRole && value.type() == typeid(bool)) {
            dbo::Transaction t(fApp->session());
            const CommentPtr& o = resultRow(index.row());
            o.modify()->set_deleted(boost::any_cast<bool>(value));
            t.commit();
            dataChanged().emit(index, this->index(index.row(), text_column));
            return true;
        }
        return BaseQM::setData(index, value, Wt::EditRole);
    }

    static Wt::WString tr(const char* key) {
        return Wt::WString::tr(key);
    }

    const int deleted_column;
    const int index_column;
    const int text_column;
};

class CommentsView : public Wt::WTableView {
public:
    CommentsView(CommentsModel* model, Wt::WContainerWidget* p=0):
        Wt::WTableView(p) {
        setModel(model);
        setSortingEnabled(false);
        resize(770, 450);
        if (model->deleted_column != -1) {
            setColumnWidth(model->deleted_column, 20);
        }
        setColumnWidth(model->index_column, 40);
        setColumnWidth(model->text_column, model->deleted_column != -1 ? 660 : 700);
        setHeaderHeight(0);
        setRowHeight(ROW_HEIGHT);
        if (model->deleted_column != -1) {
            setColumnAlignment(model->deleted_column, Wt::AlignRight | Wt::AlignTop);
        }
        setColumnAlignment(model->index_column, Wt::AlignRight | Wt::AlignTop);
        setColumnAlignment(model->text_column, Wt::AlignLeft | Wt::AlignTop);
        addStyleClass("facts-commentsview");
    }

    void goto_index(int index) {
        index = model()->rowCount() - index;
        scrollTo(model()->index(index, 0), Wt::WAbstractItemView::PositionAtTop);
    }
};

class CommentAddForm : public Wt::WTemplate {
public:
    CommentAddForm(CommentsWidget* comments):
        Wt::WTemplate(tr("facts.comment.add_template")),
        comments_(comments) {
        username_ = new Wt::WLineEdit();
        username_->setEmptyText(tr("facts.comment.Username"));
        username_->setTextSize(INPUT_SIZE);
        username_->setValidator(new Wt::WLengthValidator(MIN_INPUT_SIZE, INPUT_SIZE));
        username_->validator()->setMandatory(true);
        email_ = new Wt::WLineEdit();
        email_->setEmptyText(tr("facts.comment.Email"));
        email_->setTextSize(INPUT_SIZE);
        email_->setValidator(new Wt::WRegExpValidator(EMAIL_PATTERN));
        text_ = new Wt::WTextEdit();
        text_->setColumns(TEXT_COLUMNS);
        Wt::WPushButton* add_button = new Wt::WPushButton(tr("facts.comment.Add"));
        add_button->clicked().connect(this, &CommentAddForm::add_handler_);
        error_ = new Wt::WText();
        bindWidget("username", username_);
        bindWidget("email", email_);
        bindWidget("text", text_);
        bindWidget("button", add_button);
        bindWidget("error", error_);
    }

private:
    CommentsWidget* comments_;
    Wt::WLineEdit* username_;
    Wt::WLineEdit* email_;
    Wt::WTextEdit* text_;
    Wt::WText* error_;

    void add_handler_() {
        Wt::WValidator::State V = Wt::WValidator::Valid;
        if (username_->validate() != V || email_->validate() != V) {
            error_->setText(tr("facts.comment.Incorrect"));
            return;
        }
        int input_length = text_->text().value().size();
        if (input_length < MIN_INPUT_SIZE || input_length > MAX_TEXT_SIZE) {
            error_->setText(tr("facts.comment.Incorrect"));
            return;
        }
        dbo::Transaction t(fApp->session());
        if (fApp->is_banned()) {
            error_->setText(tr("facts.common.BannedIp"));
            return;
        }
        int index;
        if (comments_->fact()->comments().size()) {
            index = 1 + fApp->session()
                    .query<int>("select max(comment_index) from facts_comment where fact_id=?")
                    .bind(comments_->fact().id());
        } else {
            index = 1;
        }
        CommentPtr c = fApp->session().add(new Comment(CommentId(comments_->fact(), index)));
        c.modify()->set_username(username_->text());
        c.modify()->set_email(email_->text().toUTF8());
        c.modify()->set_text(text_->text());
        comments_->comment_added_handler_(); // delete this
        t.commit();
    }
};

CommentsWidget::CommentsWidget(const FactPtr& fact, Wt::WContainerWidget* p):
    Wt::WTemplate(tr("facts.comment.comments_template"), p),
    fact_(fact) {
    dbo::Transaction t(fApp->session());
    Q query = fact->comments().find();
    query.orderBy("comment_index desc");
    CommentsModel* model = new CommentsModel(query, this);
    view_ = new CommentsView(model);
    show_button_();
    bindWidget("comments", view_);
    if (fApp->admin() && !fApp->environment().ajax()) {
        bindWidget("save", new Wt::WPushButton(tr("facts.admin.Save")));
    } else {
        bindString("save", "");
    }
    int index = fApp->comment_index(fact);
    if (index != -1) {
        view_->goto_index(index);
    }
    t.commit();
}

void CommentsWidget::comment_added_handler_() {
    dbo::Transaction t(fApp->session());
    static_cast<CommentsModel*>(view_->model())->reload();
    show_button_();
    t.commit();
}

void CommentsWidget::show_button_() {
    Wt::WPushButton* add = new Wt::WPushButton(tr("facts.comment.Add"));
    add->clicked().connect(this, &CommentsWidget::show_form_);
    bindWidget("add", add);
}

void CommentsWidget::show_form_() {
    if (fApp->is_banned()) {
        bindString("add", tr("facts.common.BannedIp"));
    } else {
        bindWidget("add", new CommentAddForm(this));
    }
}

}

