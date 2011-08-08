/*
 * facts, random fact viewer
 * Copyright (C) 2011 Boris Nagaev
 *
 * facts is licensed under the GNU GPL Version 2.
 * Other versions of the GPL do not apply.
 * See the LICENSE file for terms of use.
 */

#ifndef FACTS_MODEL_COMMENT_HPP_
#define FACTS_MODEL_COMMENT_HPP_

#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/WtSqlTraits>
#include <Wt/Dbo/ptr>
#include <Wt/WDateTime>
#include <Wt/WString>
namespace dbo = Wt::Dbo;

namespace facts {
class Comment;
typedef dbo::ptr<Comment> CommentPtr;
typedef dbo::collection<CommentPtr> Comments;
}

#include "model/Fact.hpp"

namespace facts {

/** Model representing a comment to a fact */
class Comment : public dbo::Dbo<Comment> {
public:
#ifndef DOXYGEN_ONLY
    Comment();
#endif

    /** Create a new comment to be added to database */
    Comment(bool);

    /** Get the fact of the comment */
    FactPtr fact() const {
        return fact_;
    }

    /** Set the fact of the comment */
    void set_fact(FactPtr v) {
        fact_ = v;
    }

    /** Get the username of commenter */
    const Wt::WString& username() const {
        return username_;
    }

    /** Set the username of commenter */
    void set_username(const Wt::WString& v) {
        username_ = v;
    }

    /** Get the email of commenter */
    const std::string& email() {
        return email_;
    }

    /** Set the email of commenter */
    void set_email(const std::string& v) {
        email_ = v;
    }

    /** Get the text of comment */
    const Wt::WString& text() {
        return text_;
    }

    /** Set the text of comment */
    void set_text(const Wt::WString& v) {
        text_ = v;
    }

    /** Get the datetime of comment added */
    const Wt::WDateTime& when_added() {
        return when_added_;
    }

    template<class Action>
    void persist(Action& a) {
        dbo::belongsTo(a, fact_, "fact", dbo::OnDeleteCascade);
        dbo::field(a, username_, "username");
        dbo::field(a, email_, "email");
        dbo::field(a, text_, "text");
        dbo::field(a, when_added_, "when_added");
    }

    friend class Session;
private:
    FactPtr fact_;
    Wt::WString username_;
    std::string email_;
    Wt::WString text_;
    Wt::WDateTime when_added_;
};

}

#endif

