/*
 * facts, random fact viewer
 * Copyright (C) 2011 Boris Nagaev
 *
 * facts is licensed under the GNU GPL Version 2.
 * Other versions of the GPL do not apply.
 * See the LICENSE file for terms of use.
 */

#include "model/Comment.hpp"

namespace facts {

Comment::Comment()
{ }

Comment::Comment(bool):
    when_added_(Wt::WDateTime::currentDateTime())
{ }

}


