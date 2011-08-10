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
#include <Wt/WDateTime>
#include <Wt/WString>
namespace dbo = Wt::Dbo;

#include "model/models.hpp"
#include "model/Fact.hpp"

namespace facts {

/** Model representing a comment to a fact */
class Comment : public dbo::Dbo<Comment> {
public:
#ifndef DOXYGEN_ONLY
    Comment();
#endif

    /** Create a new comment to be added to database */
    Comment(const CommentId& id);

    /** Get the username of commenter */
    const Wt::WString& username() const {
        return username_;
    }

    /** Set the username of commenter */
    void set_username(const Wt::WString& v) {
        username_ = v;
    }

    /** Get the email of commenter */
    const std::string& email() const {
        return email_;
    }

    /** Set the email of commenter */
    void set_email(const std::string& v) {
        email_ = v;
    }

    /** Get the text of comment */
    const Wt::WString& text() const {
        return text_;
    }

    /** Set the text of comment */
    void set_text(const Wt::WString& v) {
        text_ = v;
    }

    /** Get the datetime of comment added */
    const Wt::WDateTime& when_added() const {
        return when_added_;
    }

    /** Get if the comment was deleted */
    bool deleted() const {
        return deleted_;
    }

    /** Set if the comment was deleted */
    void set_deleted(bool v=true) {
        deleted_ = v;
    }

    template<class Action>
    void persist(Action& a) {
        dbo::id(a, id_, "id");
        dbo::field(a, username_, "username", 100);
        dbo::field(a, email_, "email", 50);
        dbo::field(a, text_, "text");
        dbo::field(a, when_added_, "when_added", 50);
        dbo::field(a, deleted_, "deleted");
    }

    friend class Session;
private:
    CommentId id_;
    Wt::WString username_;
    std::string email_;
    Wt::WString text_;
    Wt::WDateTime when_added_;
    bool deleted_;
};

}

#endif

