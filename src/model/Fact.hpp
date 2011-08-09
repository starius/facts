/*
 * facts, random fact viewer
 * Copyright (C) 2011 Boris Nagaev
 *
 * facts is licensed under the GNU GPL Version 2.
 * Other versions of the GPL do not apply.
 * See the LICENSE file for terms of use.
 */

#ifndef FACTS_MODEL_FACT_HPP_
#define FACTS_MODEL_FACT_HPP_

#include <Wt/Dbo/Dbo>
#include <Wt/WDateTime>
namespace dbo = Wt::Dbo;

#include "model/models.hpp"
#include "model/Vote.hpp"
#include "model/Comment.hpp"

namespace facts {

/** Model representing one fact in database of facts.
*/
class Fact : public dbo::Dbo<Fact> {
public:
#ifndef DOXYGEN_ONLY
    Fact();
#endif

    /** Create a new fact to be added to database */
    Fact(bool);

    /** Set the text of the fact */
    void set_text(const Wt::WString& text) {
        text_ = text;
    }

    /** Get the text of the fact */
    const Wt::WString& text() const {
        return text_;
    }

    /** Get the datetime of creation of the fact */
    const Wt::WDateTime& when_added() const {
        return when_added_;
    }

    /** Get the score of the fact */
    int score() const {
        return score_;
    }

    /** Get the comments of the fact */
    const Comments& comments() const {
        return comments_;
    }

    template<class Action>
    void persist(Action& a) {
        dbo::field(a, text_, "text");
        dbo::field(a, when_added_, "when_added", 50);
        dbo::field(a, score_, "score");
        dbo::hasMany(a, votes_, dbo::ManyToOne, "fact");
        dbo::hasMany(a, comments_, dbo::ManyToOne, "fact");
    }

    friend class Session;
    friend class Vote;
private:
    Wt::WString text_;
    Wt::WDateTime when_added_;
    int score_;

    Votes votes_;
    Comments comments_;
};

}

#endif

