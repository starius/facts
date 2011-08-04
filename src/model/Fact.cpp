/*
 * facts, random fact viewer
 * Copyright (C) 2010 Boris Nagaev
 *
 * facts is licensed under the GNU GPL Version 2.
 * Other versions of the GPL do not apply.
 * See the LICENSE file for terms of use.
 */

#include "model/Fact.hpp"

namespace facts {

Fact::Fact() {
}

Fact::Fact(bool):
    when_added_(Wt::WDateTime::currentDateTime()) {
}

}

