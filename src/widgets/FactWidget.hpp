/*
 * facts, random fact viewer
 * Copyright (C) 2011 Boris Nagaev
 *
 * facts is licensed under the GNU GPL Version 2.
 * Other versions of the GPL do not apply.
 * See the LICENSE file for terms of use.
 */

#ifndef FACTS_WIDGETS_FACTWIDGET_HPP_
#define FACTS_WIDGETS_FACTWIDGET_HPP_

#include <Wt/WContainerWidget>
#include <Wt/WText>

#include "model/models.hpp"

namespace facts {
namespace dbo = Wt::Dbo;

/** Widget displaying one fact.
*/
class FactWidget : public Wt::WContainerWidget {
public:
    FactWidget(const FactPtr& fact, Wt::WContainerWidget* p=0);

    /** Get the fact */
    const FactPtr& fact() const {
        return fact_;
    }

private:
    FactPtr fact_;
    Wt::WText* score_;

    void set_score_();
    void vote_(short diff);
};

}

#endif

