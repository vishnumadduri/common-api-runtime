// Copyright (C) 2015 BMW Group
// Author: Lutz Bichler (lutz.bichler@bmw.de)
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef COMMONAPI_EXPORT_HPP_
#define COMMONAPI_EXPORT_HPP_

#ifdef WIN32
#define COMMONAPI_EXPORT __declspec(dllexport)
#else
#define COMMONAPI_EXPORT
#endif

#endif // COMMONAPI_EXPORT_HPP_
