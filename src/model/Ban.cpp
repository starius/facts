/*
 * facts, random fact viewer
 * Copyright (C) 2011 Boris Nagaev
 *
 * facts is licensed under the GNU GPL Version 2.
 * Other versions of the GPL do not apply.
 * See the LICENSE file for terms of use.
 */

#include "model/Ban.hpp"

namespace facts {

Ban::Ban()
{ }

Ban::Ban(const std::string& ip):
    ip_(ip),
    when_added_(Wt::WDateTime::currentDateTime()),
    banned_(true)
{ }

}

namespace Wt {
namespace Dbo {

template<>
void id(FromAnyAction&, std::string&, const std::string&, int) {
    // FIXME
}

}
}

