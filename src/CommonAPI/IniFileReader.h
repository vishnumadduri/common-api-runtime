/* Copyright (C) 2015 BMW Group
 * Author: Lutz Bichler (lutz.bichler@bmw.de)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef COMMONAPI_INIFILEREADER_H_
#define COMMONAPI_INIFILEREADER_H_

#include <map>
#include <memory>
#include <string>

namespace CommonAPI {

class IniFileReader {
public:
	class Section {
	public:
		const std::map<std::string, std::string> getMappings() const;
		std::string getValue(const std::string &_key) const;
	private:
		std::map<std::string, std::string> mappings_;

	friend class IniFileReader;
	};

	bool load(const std::string &_path);

	std::shared_ptr<Section> getSection(const std::string &_name) const;

private:
	std::map<std::string, std::shared_ptr<Section>> sections_;
};

} /* CommonAPI */

#endif /* COMMONAPI_INIFILEREADER_H_ */
