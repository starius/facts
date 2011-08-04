/*
 * facts, random fact viewer
 * Copyright (C) 2010 Boris Nagaev
 *
 * facts is licensed under the GNU GPL Version 2.
 * Other versions of the GPL do not apply.
 * See the LICENSE file for terms of use.
 */

#include <Wt/WEnvironment>

#include "Application.hpp"

namespace facts {

Application::Application(const Wt::WEnvironment& env, Server& server):
    Wt::WApplication(env), session_(server) {
}

Application* Application::instance() {
    return static_cast<Application*>(Wt::WApplication::instance());
}

}

