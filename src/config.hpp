/*
 * facts, random fact viewer
 * Copyright (C) 2011 Boris Nagaev
 *
 * facts is licensed under the GNU GPL Version 2.
 * Other versions of the GPL do not apply.
 * See the LICENSE file for terms of use.
 */

#ifndef FACTS_CONFIG_HPP_
#define FACTS_CONFIG_HPP_

/** \file Compile-time constants */

namespace facts {

const int CONNECTIONS_IN_POOL = 10;
const char* const SQLITE_DATABASE_NAME = "f.db";
const char* const ADMIN_PASSWORD = "asuperes";

}

#endif

