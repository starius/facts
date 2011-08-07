/*
 * facts, random fact viewer
 * Copyright (C) 2011 Boris Nagaev
 *
 * facts is licensed under the GNU GPL Version 2.
 * Other versions of the GPL do not apply.
 * See the LICENSE file for terms of use.
 */

#include <boost/tuple/tuple.hpp>
#include <boost/foreach.hpp>

#include <Wt/Dbo/Transaction>

#include "model/Fact.hpp"
#include "Session.hpp"

namespace facts {

Session::Session(Server& server) {
    setConnectionPool(server.pool());
    mapClass<Fact>("facts_fact");
    mapClass<Vote>("facts_vote");
}

void Session::reconsider() {
    try {
        dbo::Transaction t(*this);
        createTables();
        std::cerr << "Created database" << std::endl;
        FactPtr fact_wt = add(new Fact(true));
        fact_wt.modify()->set_text("Wt is a C++ library for developing web applications");
        std::cerr<< "and fact about Wt" << std::endl;
        FactPtr fact_facts = add(new Fact(true));
        fact_facts.modify()->set_text("Facts is a random fact viewer");
        std::cerr<< "and fact about facts" << std::endl;
        t.commit();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << "Using existing database" << std::endl;
    }
    reconsider_scores_();
}

void Session::reconsider_scores_() {
    dbo::Transaction t(*this);
    typedef boost::tuple<FactPtr, int> R;
    typedef dbo::collection<R> Rs;
    typedef dbo::Query<R> Q;
    Q q = query<R>("select F, sum(V.diff) from facts_vote V "
                   " left join facts_fact F on F.id=V.fact_id ");
    q.groupBy("F");
    Rs rs = q;
    BOOST_FOREACH (R& r, rs) {
        FactPtr& fact = r.get<0>();
        int score = r.get<1>();
        fact.modify()->score_ = score;
    }
    t.commit();
}

}

