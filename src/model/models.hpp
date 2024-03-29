/*
 * facts, random fact viewer
 * Copyright (C) 2011 Boris Nagaev
 *
 * facts is licensed under the GNU GPL Version 2.
 * Other versions of the GPL do not apply.
 * See the LICENSE file for terms of use.
 */

#ifndef FACTS_MODEL_MODELS_IDS_HPP_
#define FACTS_MODEL_MODELS_IDS_HPP_

#include <string>

#include <Wt/Dbo/WtSqlTraits>
#include <Wt/Dbo/ptr>
#include <Wt/Dbo/collection>
namespace dbo = Wt::Dbo;

const int MAX_IP_LENGTH = 45;

/** \name Declarations of all models */
/* @{ */

namespace facts {
class Fact;
class Vote;
class Comment;
class Ban;

typedef dbo::ptr<Fact> FactPtr;
typedef dbo::ptr<Vote> VotePtr;
typedef dbo::ptr<Comment> CommentPtr;
typedef dbo::ptr<Ban> BanPtr;

typedef dbo::collection<FactPtr> Facts;
typedef dbo::collection<VotePtr> Votes;
typedef dbo::collection<CommentPtr> Comments;
typedef dbo::collection<BanPtr> Bans;
}

/* @} */

/** \name Settings of Fact model */
/* @{ */

namespace Wt {
namespace Dbo {

template<>
struct dbo_traits<facts::Fact> : public dbo_default_traits {
    static const char *versionField() {
        return 0;
    }
};

}
}

/* @} */

/** \name Settings of Vote model */
/* @{ */

namespace facts {

struct VoteId {
    VoteId()
    { }

    VoteId(const FactPtr& f, const std::string& i):
        fact(f), ip(i)
    { }

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

template<>
void id(FromAnyAction&, facts::VoteId&, const std::string&, int);

}
}

/* @} */

/** \name Settings of Comment model */
/* @{ */

namespace facts {

struct CommentId {
    CommentId()
    { }

    CommentId(const FactPtr& f, int i):
        fact(f), index(i)
    { }

    FactPtr fact;
    int index;

    bool operator== (const CommentId& other) const {
        return fact==other.fact && index==other.index;
    }

    bool operator< (const CommentId& other) const {
        return fact < other.fact || (fact == other.fact && index < other.index);
    }
};

std::ostream& operator<< (std::ostream& o, const CommentId& vi);

}

namespace Wt {
namespace Dbo {

template <class Action>
void field(Action& action, facts::CommentId& id,
           const std::string& /* name */ , int /* size */ = -1) {
    belongsTo(action, id.fact, "fact", OnDeleteCascade);
    field(action, id.index, "comment_index");
}

template<>
struct dbo_traits<facts::Comment> : public dbo_default_traits {
    typedef facts::CommentId IdType;

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

template<>
void id(FromAnyAction&, facts::CommentId&, const std::string&, int);

}
}

/* @} */

/** \name Settings of Ban model */
/* @{ */

namespace Wt {
namespace Dbo {

template<>
struct dbo_traits<facts::Ban> : public dbo_default_traits {
    typedef std::string IdType;

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

template<>
void id(FromAnyAction&, std::string&, const std::string&, int);

}
}

/* @} */

#endif

