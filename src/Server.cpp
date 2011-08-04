/*
 * facts, random fact viewer
 * Copyright (C) 2010 Boris Nagaev
 *
 * facts is licensed under the GNU GPL Version 2.
 * Other versions of the GPL do not apply.
 * See the LICENSE file for terms of use.
 */

#include <boost/bind.hpp>

#include <Wt/WEnvironment>

#include "model/Fact.hpp"
#include "Session.hpp"
#include "Application.hpp"
#include "config.hpp"

namespace facts {

Wt::WApplication* createApplication(Server* server, const Wt::WEnvironment& env) {
    return new Application(env, *server);
}

Server::Server(int argc, char **argv):
    pool_(Session::new_connection(), CONNECTIONS_IN_POOL) {
    setServerConfiguration(argc, argv);
    addEntryPoint(Wt::Application, boost::bind(createApplication, this, _1), "", "/favicon.ico");
    Session s(*this);
    s.reconsider();
}

}

