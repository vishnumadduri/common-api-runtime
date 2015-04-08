// Copyright (C) 2014 BMW Group
// Author: Manfred Bathelt (manfred.bathelt@bmw.de)
// Author: Juergen Gehring (juergen.gehring@bmw.de)
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef COMMONAPI_UTILS_HPP_
#define COMMONAPI_UTILS_HPP_

#ifndef WIN32
#include <dirent.h>
#include <dlfcn.h>
#endif
#include <sys/stat.h>

#include <cstring>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iostream>

#include <locale>
#include <functional>

#ifdef WIN32
#include <xfunctional>
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // this prevents windows.h from including winsock.h, which causes duplicate definitions with winsock2.h
#endif
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace CommonAPI {

std::vector<std::string> split(const std::string& s, char delim);
void trim(std::string &_s);

} //namespace CommonAPI

#endif /* COMMONAPI_UTILS_HPP_ */
