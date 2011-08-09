/*
 * facts, random fact viewer
 * Copyright (C) 2011 Boris Nagaev
 *
 * facts is licensed under the GNU GPL Version 2.
 * Other versions of the GPL do not apply.
 * See the LICENSE file for terms of use.
 */

#ifndef FACTS_WIDGETS_COMMENTS_WIDGET_HPP_
#define FACTS_WIDGETS_COMMENTS_WIDGET_HPP_

#include <Wt/WTemplate>

#include "model/models.hpp"

namespace Wt {
class WContainerWidget;
}

namespace facts {
class CommentsView;

/** Table view of facts comments and form for addition a comment.
Allows editing (or removing) if the user has admin permissions
*/
class CommentsWidget : public Wt::WTemplate {
public:
    CommentsWidget(const FactPtr& fact, Wt::WContainerWidget* p=0);

    /** Get the fact */
    const FactPtr& fact() const {
        return fact_;
    }

    friend class CommentAddForm;
private:
    FactPtr fact_;
    CommentsView* view_;

    void comment_added_handler_();
    void show_button_();
    void show_form_();
};

}

#endif

