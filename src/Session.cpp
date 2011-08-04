/*
 * facts, random fact viewer
 * Copyright (C) 2010 Boris Nagaev
 *
 * facts is licensed under the GNU GPL Version 2.
 * Other versions of the GPL do not apply.
 * See the LICENSE file for terms of use.
 */

#include <Wt/Dbo/backend/Sqlite3>

#include "model/Fact.hpp"
#include "Session.hpp"
#include "config.hpp"

namespace facts {

Session::Session(Server& server) {
    setConnectionPool(server.pool());
    mapClass<Fact>("facts_fact");
}

dbo::SqlConnection* Session::new_connection() {
    return new dbo::backend::Sqlite3(SQLITE_DATABASE_NAME);
}

}

