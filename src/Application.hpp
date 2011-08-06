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

#include <boost/format.hpp>

#include <Wt/WApplication>

#include "Session.hpp"
#include "model/Fact.hpp"

#define fApp facts::Application::instance()

namespace facts {
class FactsWidget;

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

    /** Genarate internel path for the fact */
    std::string fact_path(FactPtr fact) const;

private:
    Session session_;
    mutable boost::format fact_path_format_;
    FactsWidget* facts_;

    void path_changed_handler_();
};

}

#endif

