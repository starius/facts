/*
 * facts, random fact viewer
 * Copyright (C) 2010 Boris Nagaev
 *
 * facts is licensed under the GNU GPL Version 2.
 * Other versions of the GPL do not apply.
 * See the LICENSE file for terms of use.
 */

#include "Server.hpp"

#include "Application.hpp"
#include "model/Fact.hpp"
#include "widgets/FactWidget.hpp"

int main(int argc, char **argv) {
    facts::Server server(argc, argv);
    if (server.start()) {
        Wt::WServer::waitForShutdown();
        server.stop();
    }
}

