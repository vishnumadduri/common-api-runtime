// Copyright (C) 2015 BMW Group
// Author: Lutz Bichler (lutz.bichler@bmw.de)
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#error "Only <CommonAPI/CommonAPI.h> can be included directly, this file may disappear or change contents."
#endif

#ifndef COMMONAPI_RUNTIME_HPP_
#define COMMONAPI_RUNTIME_HPP_

#include <map>
#include <memory>
#include <mutex>
#include <set>

#include <CommonAPI/Factory.hpp>

namespace CommonAPI {

class MainLoopContext;
class Proxy;
class ProxyManager;
class StubBase;

template<template<typename ...> class _ProxyType, template<typename> class _AttributeExtension>
struct DefaultAttributeProxyFactoryHelper;

template<template<typename ...> class _ProxyClass, template<typename> class _AttributeExtension>
std::shared_ptr<
	typename DefaultAttributeProxyFactoryHelper<_ProxyClass, _AttributeExtension>::class_t
> createProxyWithDefaultAttributeExtension(
	const std::string &_participant, const std::string &_domain);

class Runtime {
public:
	static std::shared_ptr<Runtime> get();

	Runtime();
	virtual ~Runtime();

    template<template<typename ...> class _ProxyClass, typename ... _AttributeExtensions>
    std::shared_ptr<
        _ProxyClass<_AttributeExtensions...>
    >
    buildProxy(const std::string &_domain,
               const std::string &_instance,
               const ConnectionId &_connectionId = COMMONAPI_DEFAULT_CONNECTION_ID) {

        std::shared_ptr<Proxy> abstractProxy
        	= createProxy(_domain, _ProxyClass<_AttributeExtensions...>::getInterface(), _instance, _connectionId);

        if (abstractProxy) {
            auto returnProxy = std::make_shared<
                _ProxyClass<_AttributeExtensions...>
            >(abstractProxy);

            return returnProxy;
        }
        else {
            return nullptr;
        }
    }

    template<template<typename ...> class _ProxyClass, typename ... _AttributeExtensions>
    std::shared_ptr<
        _ProxyClass<_AttributeExtensions...>
    >
    buildProxy(const std::string &_domain,
               const std::string &_instance,
               std::shared_ptr<MainLoopContext> _context) {

        std::shared_ptr<Proxy> abstractProxy
        	= createProxy(_domain, _ProxyClass<_AttributeExtensions...>::getInterface(), _instance, _context);

        if (abstractProxy) {
            auto returnProxy = std::make_shared<
                _ProxyClass<_AttributeExtensions...>
            >(abstractProxy);

            return returnProxy;
        }
        else {
            return nullptr;
        }
    }

    template<typename _Stub>
	bool registerService(const std::string &_domain,
						 const std::string &_instance,
						 std::shared_ptr<_Stub> _service,
						 const ConnectionId &_connectionId = COMMONAPI_DEFAULT_CONNECTION_ID) {
		return registerStub(_domain, _Stub::StubInterface::getInterface(), _instance, _service, _connectionId);
	}

    template<typename _Stub>
    bool registerService(const std::string &_domain,
    					 const std::string &_instance,
    					 std::shared_ptr<_Stub> _service,
    					 std::shared_ptr<MainLoopContext> _context) {
    	return registerStub(_domain, _Stub::StubInterface::getInterface(), _instance, _service, _context);
    }

    bool registerFactory(const std::string &_ipc, std::shared_ptr<Factory> _factory);
    bool unregisterFactory(const std::string &_ipc);

private:
	void init();
	bool readConfiguration();
	bool splitAddress(const std::string &, std::string &, std::string &, std::string &);

	std::shared_ptr<Proxy> createProxy(const std::string &, const std::string &, const std::string &,
									   const ConnectionId &);
	std::shared_ptr<Proxy> createProxy(const std::string &, const std::string &, const std::string &,
									   std::shared_ptr<MainLoopContext>);

	bool registerStub(const std::string &, const std::string &, const std::string &,
						  std::shared_ptr<StubBase>, const ConnectionId &);
	bool registerStub(const std::string &, const std::string &, const std::string &,
					  std::shared_ptr<StubBase>, std::shared_ptr<MainLoopContext>);

	std::string getLibrary(const std::string &, const std::string &, const std::string &, bool);
	bool loadLibrary(const std::string &);

private:
	std::map<std::string, std::shared_ptr<Factory>> factories_;
	std::map<std::string, std::map<bool, std::string>> libraries_;
	std::set<std::string> loadedLibraries_; // Library name

	std::string defaultBinding_;
	std::string defaultFolder_;
	std::string defaultConfig_;

	std::mutex factoriesMutex_;


friend class ProxyManager;
};

} // namespace CommonAPI

#endif // COMMONAPI_RUNTIME_HPP_
