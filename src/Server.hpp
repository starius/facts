/*
 * facts, random fact viewer
 * Copyright (C) 2011 Boris Nagaev
 *
 * facts is licensed under the GNU GPL Version 2.
 * Other versions of the GPL do not apply.
 * See the LICENSE file for terms of use.
 */

#ifndef FACTS_SERVER_HPP_
#define FACTS_SERVER_HPP_

#include <Wt/WServer>
#include <Wt/Dbo/FixedSqlConnectionPool>

namespace facts {
namespace dbo = Wt::Dbo;

/** Descendant of WApplication, used by all sessions of site
*/
class Server : public Wt::WServer {
public:
    Server(int argc, char **argv);

    /** Return pool of database connections */
    dbo::FixedSqlConnectionPool& pool() {
        return pool_;
    }

private:
    dbo::FixedSqlConnectionPool pool_;

    dbo::SqlConnection* new_connection_();
};

}

#endif



