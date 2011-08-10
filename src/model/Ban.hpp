/*
 * facts, random fact viewer
 * Copyright (C) 2011 Boris Nagaev
 *
 * facts is licensed under the GNU GPL Version 2.
 * Other versions of the GPL do not apply.
 * See the LICENSE file for terms of use.
 */

#ifndef FACTS_MODEL_BAN_HPP_
#define FACTS_MODEL_BAN_HPP_

#include <Wt/Dbo/Dbo>
#include <Wt/WDateTime>
namespace dbo = Wt::Dbo;

#include "model/models.hpp"
#include "model/Fact.hpp"

namespace facts {

/** Model representing the banned ip address */
class Ban : public dbo::Dbo<Ban> {
public:
#ifndef DOXYGEN_ONLY
    Ban();
#endif

    /** Create a new ban to be added to the database */
    Ban(const std::string& ip);

    /** Get if the ip is banned */
    bool banned() const {
        return banned_;
    }

    /** Set if the ip is banned.
    By default new instance of Ban is banned
    */
    void set_banned(bool v=true) {
        banned_ = v;
    }

    template<class Action>
    void persist(Action& a) {
        dbo::id(a, ip_, "ip", MAX_IP_LENGTH);
        dbo::field(a, when_added_, "when_added");
        dbo::field(a, banned_, "banned");
    }

    friend class Session;
private:
    std::string ip_;
    Wt::WDateTime when_added_;
    bool banned_;
};

}

#endif

