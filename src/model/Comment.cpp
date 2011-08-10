/*
 * facts, random fact viewer
 * Copyright (C) 2011 Boris Nagaev
 *
 * facts is licensed under the GNU GPL Version 2.
 * Other versions of the GPL do not apply.
 * See the LICENSE file for terms of use.
 */

#include "model/Comment.hpp"
#include "model/Fact.hpp"

namespace facts {

Comment::Comment()
{ }

Comment::Comment(const CommentId& id):
    id_(id),
    when_added_(Wt::WDateTime::currentDateTime()),
    deleted_(false)
{ }

std::ostream& operator<< (std::ostream& o, const CommentId& vi) {
    return o << "CommentId(" << vi.fact << ", " << vi.index << ")";
}

}

namespace Wt {
namespace Dbo {

template<>
void id(FromAnyAction&, facts::VoteId&, const std::string&, int) {
    // FIXME
}

}
}

