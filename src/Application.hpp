/*
 * facts, random fact viewer
 * Copyright (C) 2011 Boris Nagaev
 *
 * facts is licensed under the GNU GPL Version 2.
 * Other versions of the GPL do not apply.
 * See the LICENSE file for terms of use.
 */

#ifndef FACTS_APPLICATION_HPP_
#define FACTS_APPLICATION_HPP_

#include <Wt/WApplication>

#include "Session.hpp"

#define fApp facts::Application::instance()

namespace facts {

/** Descendant of WApplication, used by all sessions of site
*/
class Application : public Wt::WApplication {
public:
    Application(const Wt::WEnvironment& env, Server& server);

    /** Return active application */
    static Application* instance();

    /** Return the database session */
    Session& session() {
        return session_;
    }

private:
    Session session_;
};

}

#endif

