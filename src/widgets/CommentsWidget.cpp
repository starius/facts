/*
 * acts, random fact viewer
 * Copyright (C) 2011 Boris Nagaev
 *
 * facts is licensed under the GNU GPL Version 2.
 * Other versions of the GPL do not apply.
 * See the LICENSE file for terms of use.
 */

#include <Wt/WContainerWidget>
#include <Wt/WTableView>
#include <Wt/Dbo/Transaction>
#include <Wt/Dbo/Query>
#include <Wt/Dbo/QueryModel>
#include <Wt/WTextEdit>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>

#include "widgets/CommentsWidget.hpp"
#include "model/Fact.hpp"
#include "model/Comment.hpp"
#include "Application.hpp"

namespace facts {

typedef CommentPtr Result;
typedef dbo::Query<Result> Q;
typedef dbo::QueryModel<Result> BaseQM;

const int INDEX_COLUMN = 0;
const int TEXT_COLUMN = 1;
const int USERNAME_COLUMN = 2;
const int EMAIL_COLUMN = 3;

const int INPUT_SIZE = 25;
const int TEXT_COLUMNS = 80;
const int ROW_HEIGHT = 140;

class CommentsModel : public BaseQM {
public:
    CommentsModel(const Q& query, Wt::WObject *parent=0) :
        BaseQM(parent) {
        setQuery(query);
        addColumn("comment_index", "");
        Wt::WFlags<Wt::ItemFlag> text_flags = Wt::ItemIsXHTMLText;
        if (fApp->admin()) {
            text_flags |= Wt::ItemIsEditable;
        }
        addColumn("text", "", text_flags);
        if (fApp->admin()) {
            addColumn("username", "", Wt::ItemIsEditable);
            addColumn("email", "", Wt::ItemIsEditable);
        }
    }

    boost::any data(const Wt::WModelIndex& index,
                    int role=Wt::DisplayRole) const {
        dbo::Transaction t(fApp->session());
        const CommentPtr& o = resultRow(index.row());
        if (role == Wt::InternalPathRole && index.column() == INDEX_COLUMN) {
            return fApp->comment_path(o);
        } else if (role == Wt::DisplayRole && index.column() == INDEX_COLUMN) {
            return tr("facts.common.id_format").arg(o.id().index);
        } else if (role == Wt::DisplayRole && index.column() == TEXT_COLUMN) {
            return tr("facts.comment.format").arg(o->username()).arg(o->email())
                   .arg(o->when_added().toString()).arg(o->text());
        }
        return BaseQM::data(index, role);
    }

    static Wt::WString tr(const char* key) {
        return Wt::WString::tr(key);
    }
};

class CommentsView : public Wt::WTableView {
public:
    CommentsView(CommentsModel* model, Wt::WContainerWidget* p=0):
        Wt::WTableView(p) {
        setModel(model);
        setSortingEnabled(false);
        resize(770, 450);
        setColumnWidth(INDEX_COLUMN, 40);
        setColumnWidth(TEXT_COLUMN, 500);
        if (fApp->admin()) {
            setColumnWidth(USERNAME_COLUMN, 100);
            setColumnWidth(EMAIL_COLUMN, 100);
        }
        setHeaderHeight(0);
        setRowHeight(ROW_HEIGHT);
        setColumnAlignment(INDEX_COLUMN, Wt::AlignRight | Wt::AlignTop);
        setColumnAlignment(TEXT_COLUMN, Wt::AlignLeft | Wt::AlignTop);
        addStyleClass("facts-commentsview");
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
        email_ = new Wt::WLineEdit();
        email_->setEmptyText(tr("facts.comment.Email"));
        email_->setTextSize(INPUT_SIZE);
        text_ = new Wt::WTextEdit();
        text_->setColumns(TEXT_COLUMNS);
        Wt::WPushButton* add_button = new Wt::WPushButton(tr("facts.comment.Add"));
        add_button->clicked().connect(this, &CommentAddForm::add_handler_);
        bindWidget("username", username_);
        bindWidget("email", email_);
        bindWidget("text", text_);
        bindWidget("button", add_button);
        bindString("error", "");
    }

private:
    CommentsWidget* comments_;
    Wt::WLineEdit* username_;
    Wt::WLineEdit* email_;
    Wt::WTextEdit* text_;

    void add_handler_() {
        dbo::Transaction t(fApp->session());
        // FIXME check!
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
    CommentAddForm* add = new CommentAddForm(this);
    bindWidget("add", add);
}

}

