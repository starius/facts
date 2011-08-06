/*
 * facts, random fact viewer
 * Copyright (C) 2010 Boris Nagaev
 *
 * facts is licensed under the GNU GPL Version 2.
 * Other versions of the GPL do not apply.
 * See the LICENSE file for terms of use.
 */

#include <Wt/Dbo/backend/Sqlite3>
#include <Wt/Dbo/Transaction>

#include "model/Fact.hpp"
#include "Session.hpp"
#include "config.hpp"

namespace facts {

Session::Session(Server& server) {
    setConnectionPool(server.pool());
    mapClass<Fact>("facts_fact");
    mapClass<Vote>("facts_vote");
}

void Session::reconsider() {
    try {
        dbo::Transaction t(*this);
        createTables();
        std::cerr << "Created database" << std::endl;
        FactPtr fact_wt = add(new Fact(true));
        fact_wt.modify()->set_text("Wt is a C++ library for developing web applications");
        std::cerr<< "and fact about Wt" << std::endl;
        FactPtr fact_facts = add(new Fact(true));
        fact_facts.modify()->set_text("Facts is a random fact viewer");
        std::cerr<< "and fact about facts" << std::endl;
        t.commit();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << "Using existing database" << std::endl;
    }
}

dbo::SqlConnection* Session::new_connection() {
    return new dbo::backend::Sqlite3(SQLITE_DATABASE_NAME);
}

}

