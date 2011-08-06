/*
 * facts, random fact viewer
 * Copyright (C) 2011 Boris Nagaev
 *
 * facts is licensed under the GNU GPL Version 2.
 * Other versions of the GPL do not apply.
 * See the LICENSE file for terms of use.
 */

#ifndef FACTS_SESSION_HPP_
#define FACTS_SESSION_HPP_

#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/ptr>
#include <Wt/Dbo/SqlConnection>
namespace dbo = Wt::Dbo;

#include "model/Fact.hpp"
#include "Server.hpp"

namespace facts {

/** Descendant of dbo::Session.
*/
class Session : public dbo::Session {
public:
    Session(Server& server);

    /** Create schema, add several facts */
    void reconsider();

    /** Return new database connection */
    static dbo::SqlConnection* new_connection();

private:
    void reconsider_scores_();
};

}

#endif

