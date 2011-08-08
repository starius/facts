/*
 * facts, random fact viewer
 * Copyright (C) 2011 Boris Nagaev
 *
 * facts is licensed under the GNU GPL Version 2.
 * Other versions of the GPL do not apply.
 * See the LICENSE file for terms of use.
 */

#include <boost/bind.hpp>

#include <Wt/WEnvironment>
#include <Wt/Dbo/backend/Sqlite3>
namespace dbo = Wt::Dbo;

#include "Server.hpp"
#include "model/Fact.hpp"
#include "Session.hpp"
#include "Application.hpp"
#include "config.hpp"

namespace facts {

Wt::WApplication* createApplication(Server* server, const Wt::WEnvironment& env) {
    return new Application(env, *server);
}

Server::Server(int argc, char **argv):
    // config should be set before running new_connection_() since it needs approot
    pool_((setServerConfiguration(argc, argv), new_connection_()), CONNECTIONS_IN_POOL) {
    addEntryPoint(Wt::Application, boost::bind(createApplication, this, _1), "", "/favicon.ico");
    Session s(*this);
    s.reconsider();
}

dbo::SqlConnection* Server::new_connection_() {
    return new dbo::backend::Sqlite3(appRoot() + SQLITE_DATABASE_NAME);
}

}

