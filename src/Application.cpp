/*
 * facts, random fact viewer
 * Copyright (C) 2011 Boris Nagaev
 *
 * facts is licensed under the GNU GPL Version 2.
 * Other versions of the GPL do not apply.
 * See the LICENSE file for terms of use.
 */

#include <Wt/WEnvironment>
#include <Wt/Dbo/Transaction>
namespace dbo = Wt::Dbo;

#include "Application.hpp"
#include "model/Fact.hpp"
#include "model/Ban.hpp"
#include "widgets/FactsWidget.hpp"

namespace facts {

Application::Application(const Wt::WEnvironment& env, Server& server):
    Wt::WApplication(env), session_(server),
    fact_path_format_("/fact/%i/"),
    comment_path_format_("/fact/%i/comment/%i/"),
    ip_path_format_("/admin/ip/%s/"),
    admin_(false) {
    init_counters_();
    messageResourceBundle().use(Wt::WApplication::appRoot() +
                                "locales/facts");
    setCssTheme("polished");
    useStyleSheet("css/facts.css");
    facts_ = new FactsWidget(root());
    internalPathChanged().connect(this, &Application::path_changed_handler_);
    facts_->set_random_fact();
    WApplication::setInternalPath(environment().internalPath(), true);
}

Application* Application::instance() {
    return static_cast<Application*>(Wt::WApplication::instance());
}

std::string Application::read_config(const std::string& name) {
    std::string result;
    readConfigurationProperty(name, result);
    return result;
}

std::string Application::fact_path(const FactPtr& fact) const {
    return str(fact_path_format_ % fact.id());
}

std::string Application::comment_path(const CommentPtr& comment) const {
    return str(comment_path_format_ % comment.id().fact.id() % comment.id().index);
}

int Application::comment_index(const FactPtr& fact) const {
    std::string comment_base = fact_path(fact) + "comment/";
    if (!internalPathMatches(comment_base)) {
        return -1;
    }
    try {
        std::string comment_str = internalPathNextPart(comment_base);
        return boost::lexical_cast<int>(comment_str);
    } catch (...) {
        return -1;
    }
}

std::string Application::admin_path() const {
    return "/admin/";
}

std::string Application::ip_path(const std::string& ip) const {
    return str(ip_path_format_ % ip);
}

std::string Application::ip_from_path() const {
    if (internalPathMatches("/admin/ip/")) {
        return internalPathNextPart("/admin/ip/");
    }
    return "";
}

std::string Application::random_fact_path() const {
    return "/fact/random/";
}

std::string Application::prev_fact_path() const {
    return "/fact/prev/";
}

std::string Application::next_fact_path() const {
    return "/fact/next/";
}

void Application::setInternalPath(const std::string& path) {
    if (!internalPathMatches(path)) {
        WApplication::setInternalPath(path);
    }
}

bool Application::is_banned() const {
    dbo::Transaction t(session_);
    bool result = false;
    try {
        BanPtr ban = session_.load<Ban>(environment().clientAddress());
        if (ban->banned()) {
            result = true;
        }
    } catch (dbo::ObjectNotFoundException)
    { }
    t.commit();
    return result;
}

void Application::path_changed_handler_() {
    dbo::Transaction t(session());
    std::string section = internalPathNextPart("/");
    if (section == "fact") {
        std::string fact_str = internalPathNextPart("/fact/");
        if (fact_str == "random") {
            facts_->set_random_fact();
        } else if (fact_str == "prev") {
            facts_->set_prev_fact();
        } else if (fact_str == "next") {
            facts_->set_next_fact();
        } else {
            try {
                int fact_id = boost::lexical_cast<int>(fact_str);
                FactPtr fact = session().load<Fact>(fact_id);
                facts_->set_fact(fact);
            } catch (...)
            { }
        }
    }
    if (section == "admin") {
        facts_->try_admin_enter();
    }
    t.commit();
}

void Application::init_counters_() {
    require("http://userapi.com/js/api/openapi.js?34", "VK");
    doJavaScript("VK.init({apiId: " + read_config("API_ID") + ", onlyWidgets: true});");
    require("http://platform.twitter.com/widgets.js");
}

}

