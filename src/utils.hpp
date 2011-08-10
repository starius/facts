/*
 * facts, random fact viewer
 * Copyright (C) 2011 Boris Nagaev
 *
 * facts is licensed under the GNU GPL Version 2.
 * Other versions of the GPL do not apply.
 * See the LICENSE file for terms of use.
 */

#ifndef FACTS_UTILS_HPP_
#define FACTS_UTILS_HPP_

#include <string>

namespace Wt {
class WInteractWidget;
}

namespace facts {

/** Create clickable image (src), emitting (href) internal path.
If parent!=0, add resulting widget to the parent.
The function takes into account availability of ajax.
*/
Wt::WInteractWidget* a_img(const std::string& src, const std::string& href,
                           Wt::WContainerWidget* parent=0);

}

#endif

