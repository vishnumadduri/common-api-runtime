/* Copyright (C) 2013 BMW Group
 * Author: Manfred Bathelt (manfred.bathelt@bmw.de)
 * Author: Juergen Gehring (juergen.gehring@bmw.de)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#error "Only <CommonAPI/CommonAPI.h> can be included directly, this file may disappear or change contents."
#endif

#ifndef COMMONAPI_PROXY_MANAGER_HPP_
#define COMMONAPI_PROXY_MANAGER_HPP_

#include <functional>
#include <future>
#include <string>
#include <vector>

#include <CommonAPI/Event.hpp>
#include <CommonAPI/Proxy.hpp>
#include <CommonAPI/Types.hpp>

namespace CommonAPI {

class ProxyManager {
public:
    typedef std::function<void(const CallStatus &, const std::vector<std::string> &)> GetAvailableInstancesCallback;
    typedef std::function<void(const CallStatus &, const AvailabilityStatus &)> GetInstanceAvailabilityStatusCallback;

    typedef Event<std::string, AvailabilityStatus> InstanceAvailabilityStatusChangedEvent;

    ProxyManager() = default;
    ProxyManager(ProxyManager &&) = delete;
    ProxyManager(const ProxyManager &) = delete;

    virtual ~ProxyManager() {}

    virtual const std::string &getDomain() const = 0;
    virtual const std::string &getInterface() const = 0;
    virtual const ConnectionId &getConnectionId() const = 0;

    virtual void getAvailableInstances(CommonAPI::CallStatus&, std::vector<std::string>& availableInstances) = 0;
	virtual std::future<CallStatus> getAvailableInstancesAsync(GetAvailableInstancesCallback callback) = 0;

    virtual void getInstanceAvailabilityStatus(const std::string& instanceAddress,
                                               CallStatus& callStatus,
                                               AvailabilityStatus& availabilityStatus) = 0;

    virtual std::future<CallStatus> getInstanceAvailabilityStatusAsync(const std::string&,
                                                                       GetInstanceAvailabilityStatusCallback callback) = 0;

    virtual InstanceAvailabilityStatusChangedEvent& getInstanceAvailabilityStatusChangedEvent() = 0;

    template<template<typename ...> class _ProxyClass, typename ... _AttributeExtensions>
    std::shared_ptr<_ProxyClass<_AttributeExtensions...> >
    buildProxy(const std::string &_instance) {
        std::shared_ptr<Proxy> proxy = createProxy(getDomain(), getInterface(), _instance, getConnectionId());
        if (proxy) {
            return std::make_shared<_ProxyClass<_AttributeExtensions...>>(proxy);
        }
        return NULL;

    }

protected:
    std::shared_ptr<Proxy> createProxy(const std::string &,
    								   const std::string &,
									   const std::string &,
									   const ConnectionId &_connection) const;
};

} // namespace CommonAPI

#endif // COMMONAPI_PROXY_MANAGER_HPP_