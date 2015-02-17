// Copyright (C) 2015 BMW Group
// Author: Lutz Bichler (lutz.bichler@bmw.de)
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef COMMONAPI_ADDRESS_HPP_
#define COMMONAPI_ADDRESS_HPP_

#include <iostream>
#include <string>

namespace CommonAPI {

class Address {
public:
	Address() = default;
	Address(const std::string &_address);
	Address(const std::string &_domain,
			const std::string &_interface,
			const std::string &_instance);
	Address(const Address &_source);
	virtual ~Address();

	bool operator<(const Address &_other) const;

	std::string getAddress() const;
	void setAddress(const std::string &_address);

	const std::string &getDomain() const;
	void setDomain(const std::string &_domain);

	const std::string &getInterface() const;
	void setInterface(const std::string &_interface);

	const std::string &getInstance() const;
	void setInstance(const std::string &_instance);

private:
	std::string domain_;
	std::string interface_;
	std::string instance_;

friend std::ostream &operator<<(std::ostream &_out, const Address &_address);
};

} // namespace CommonAPI

#endif // COMMONAPI_ADDRESS_HPP_
