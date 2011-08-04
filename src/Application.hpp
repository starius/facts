/*
 * facts, random fact viewer
 * Copyright (C) 2010 Boris Nagaev
 *
 * facts is licensed under the GNU GPL Version 2.
 * Other versions of the GPL do not apply.
 * See the LICENSE file for terms of use.
 */

#ifndef FACTS_APPLICATION_HPP_
#define FACTS_APPLICATION_HPP_

#include <Wt/WApplication>

namespace facts {

/** Descendant of WApplication, used by all sessions of site
*/
class Application : public Wt::WApplication {
public:
    Application();

private:
};

}

#endif



