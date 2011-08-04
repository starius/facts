/*
 * facts, random fact viewer
 * Copyright (C) 2010 Boris Nagaev
 *
 * facts is licensed under the GNU GPL Version 2.
 * Other versions of the GPL do not apply.
 * See the LICENSE file for terms of use.
 */

#include "model/Fact.hpp"
#include "Session.hpp"
#include "config.hpp"

namespace facts {

Server::Server(int argc, char **argv):
    pool_(Session::new_connection(), CONNECTIONS_IN_POOL) {
    setServerConfiguration(argc, argv);
}

}

