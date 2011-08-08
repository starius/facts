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

Comment::Comment(const CommentId& id):
    id_(id),
    when_added_(Wt::WDateTime::currentDateTime())
{ }

std::ostream& operator<< (std::ostream& o, const CommentId& vi) {
    return o << "CommentId(" << vi.fact << ", " << vi.index << ")";
}

}

