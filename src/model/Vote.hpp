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
#include <Wt/Dbo/WtSqlTraits>
#include <Wt/Dbo/ptr>
#include <Wt/WDateTime>
namespace dbo = Wt::Dbo;

namespace facts {
class Vote;
typedef dbo::ptr<Vote> VotePtr;
typedef dbo::collection<VotePtr> Votes;
}

#include "model/Fact.hpp"

namespace facts {

struct VoteId {
    FactPtr fact;
    std::string ip;

    bool operator== (const VoteId& other) const {
        return fact==other.fact && ip==other.ip;
    }

    bool operator< (const VoteId& other) const {
        return fact < other.fact || (fact == other.fact && ip < other.ip);
    }
};

std::ostream& operator<< (std::ostream& o, const VoteId& vi);

}

namespace Wt {
namespace Dbo {

template <class Action>
void field(Action& action, facts::VoteId& vote_id,
           const std::string& /* name */ , int /* size */ = -1) {
    belongsTo(action, vote_id.fact, "fact", OnDeleteCascade);
    const int MAX_IP_LENGTH = 45;
    field(action, vote_id.ip, "ip", MAX_IP_LENGTH);
}

template<>
struct dbo_traits<facts::Vote> : public dbo_default_traits {
    typedef facts::VoteId IdType;

    static IdType invalidId() {
        return IdType();
    }

    static const char *surrogateIdField() {
        return 0;
    }

    static const char *versionField() {
        return 0;
    }
};

}
}

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
    Vote(FactPtr fact, const std::string& ip, short diff);

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
        dbo::id(a, vote_id_, "id");
        dbo::field(a, when_added_, "when_added");
        dbo::field(a, diff_, "diff");
    }

private:
    VoteId vote_id_;
    Wt::WDateTime when_added_;
    short diff_;
};

}

#endif

