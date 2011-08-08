/*
 * facts, random fact viewer
 * Copyright (C) 2011 Boris Nagaev
 *
 * facts is licensed under the GNU GPL Version 2.
 * Other versions of the GPL do not apply.
 * See the LICENSE file for terms of use.
 */

#include "model/Vote.hpp"

namespace facts {

Vote::Vote() {
}

Vote::Vote(const VoteId& id, short diff):
    id_(id),
    when_added_(Wt::WDateTime::currentDateTime()),
    diff_(diff) {
    id_.fact.modify()->score_ += diff;
}

std::ostream& operator<< (std::ostream& o, const VoteId& vi) {
    return o << "VoteId(" << vi.fact << ", " << vi.ip << ")";
}

}

