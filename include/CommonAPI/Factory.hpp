/* Copyright (C) 2015 BMW Group
 * Author: Lutz Bichler (lutz.bichler@bmw.de)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef COMMONAPI_FACTORY_HPP_
#define COMMONAPI_FACTORY_HPP_

#include <memory>
#include <vector>

namespace CommonAPI {

class MainLoopContext;
class Proxy;
class StubBase;

class Factory {
public:
    typedef std::function<void(std::vector<std::string> &)> AvailableInstancesCbk_t;
    typedef std::function<void(bool)> InstanceAliveCbk_t;

    virtual ~Factory() {};

	virtual std::shared_ptr<Proxy> createProxy(const std::string &_domain,
									   	   	   const std::string &_interface,
									   	   	   const std::string &_instance,
									   	   	   std::shared_ptr<MainLoopContext> mainLoopContext) = 0;

	virtual bool registerStub(const std::string &_domain,
						   	  const std::string &_interface,
							  const std::string &_instance,
						 	  std::shared_ptr<StubBase> _stub,
						 	  std::shared_ptr<MainLoopContext> mainLoopContext) = 0;
};

} /* namespace CommonAPI */

#endif /* COMMONAPI_FACTORY_HPP_ */
