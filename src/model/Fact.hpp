/*
 * facts, random fact viewer
 * Copyright (C) 2010 Boris Nagaev
 *
 * facts is licensed under the GNU GPL Version 2.
 * Other versions of the GPL do not apply.
 * See the LICENSE file for terms of use.
 */

#ifndef FACTS_MODEL_FACT_HPP_
#define FACTS_MODEL_FACT_HPP_

#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/WtSqlTraits>
#include <Wt/Dbo/ptr>
#include <Wt/WDateTime>
namespace dbo = Wt::Dbo;

namespace facts {
class Fact;
typedef dbo::ptr<Fact> FactPtr;
}

namespace facts {

/** Model representing one fact in database of facts.
*/
class Fact : public dbo::Dbo<Fact> {
public:
#ifndef DOXYGEN_ONLY
    Fact();
#endif
    Fact(bool);

    template<class Action>
    void persist(Action& a) {
        dbo::field(a, text_, "text");
        dbo::field(a, when_added_, "when_added");
    }

private:
    Wt::WString text_;
    Wt::WDateTime when_added_;
};

}

#endif

