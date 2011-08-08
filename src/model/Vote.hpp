/*
 * facts, random fact viewer
 * Copyright (C) 2011 Boris Nagaev
 *
 * facts is licensed under the GNU GPL Version 2.
 * Other versions of the GPL do not apply.
 * See the LICENSE file for terms of use.
 */

#ifndef FACTS_MODEL_VOTE_HPP_
#define FACTS_MODEL_VOTE_HPP_

#include <Wt/Dbo/Dbo>
#include <Wt/WDateTime>
namespace dbo = Wt::Dbo;

#include "model/models.hpp"
#include "model/Fact.hpp"

namespace facts {

/** Model representing one vote by one IP address.
*/
class Vote : public dbo::Dbo<Vote> {
public:
#ifndef DOXYGEN_ONLY
    Vote();
#endif

    /** Create a new vote to be added to database.
    This method modifies the fact, changing it's score.
    */
    Vote(const VoteId& id, short diff);

    /** Get the datetime of creation of the vote */
    const Wt::WDateTime& when_added() const {
        return when_added_;
    }

    /** Get the diff of the vote */
    int diff() const {
        return diff_;
    }

    template<class Action>
    void persist(Action& a) {
        dbo::id(a, id_, "id");
        dbo::field(a, when_added_, "when_added", 50);
        dbo::field(a, diff_, "diff");
    }

    friend class Session;
private:
    VoteId id_;
    Wt::WDateTime when_added_;
    short diff_;
};

}

#endif

