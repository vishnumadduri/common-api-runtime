/* Copyright (C) 2015 BMW Group
 * Author: Lutz Bichler (lutz.bichler@bmw.de)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <dlfcn.h>
#include <unistd.h>
#include <sys/stat.h>

#include <algorithm>
#include <fstream>
#include <sstream>

#include "Factory.h"
#include "Logger.h"
#include "Runtime.h"
#include "Utils.h"

namespace CommonAPI {

std::shared_ptr<Runtime> Runtime::get() {
	static std::shared_ptr<Runtime> theRuntime = std::make_shared<Runtime>();
	return theRuntime;
}

Runtime::Runtime() {
	init();
}

Runtime::~Runtime() {
	// intentionally left empty
}

bool
Runtime::registerFactory(const std::string &_ipc, std::shared_ptr<Factory> _factory) {
	Logger::log("Registering factory for IPC=", _ipc);
	// TODO: add a lock
	bool isRegistered(false);
	auto foundFactory = factories_.find(_ipc);
	if (foundFactory == factories_.end()) {
		factories_[_ipc] = _factory;
		isRegistered = true;
	}
	return isRegistered;
}

bool
Runtime::unregisterFactory(const std::string &_ipc) {
	return false;
}

/*
 * Private
 */
void Runtime::init() {
	// Determine default configuration file
	const char *config = getenv("COMMONAPI_DEFAULT_CONFIG");
	if (config)
		defaultConfig_ = config;
	else
		defaultConfig_ = COMMONAPI_DEFAULT_CONFIG;

	// TODO: evaluate return parameter and decide what to do
	(void)readConfiguration();

	// Determine default ipc & shared library folder
	const char *ipc = getenv("COMMONAPI_DEFAULT_IPC");
	if (ipc)
		defaultIpc_ = ipc;
	else
		defaultIpc_ = COMMONAPI_DEFAULT_IPC;

	const char *folder = getenv("COMMONAPI_DEFAULT_FOLDER");
	if (folder)
		defaultFolder_ = folder;
	else
		defaultFolder_ = COMMONAPI_DEFAULT_FOLDER;

	// Log settings
	Logger::log("Using default IPC=", defaultIpc_);
	Logger::log("Using default shared library folder=", defaultFolder_);
	Logger::log("Using default configuration file=", defaultConfig_);
}

bool
Runtime::readConfiguration() {
#define MAX_PATH_LEN 255
	std::string config;
	char currentDirectory[MAX_PATH_LEN];
	if (getcwd(currentDirectory, MAX_PATH_LEN)) {
		config = currentDirectory;
		config += "/commonapi.cfg";

		struct stat s;
		if (stat(config.c_str(), &s) != 0) {
			config = defaultConfig_;
		}
	}

	std::ifstream configStream(config);
	if (configStream.is_open()) {
		Logger::log("Loading configuration from ", config);

		enum { DEFAULT, PROXY, STUB, UNKNOWN } loadType(UNKNOWN);
		while (!configStream.eof()) {
			std::string line;
			std::getline(configStream, line);
			trim(line);

			if (line == "[default]") {
				loadType = DEFAULT;
			} else if (line == "[proxy]") {
				loadType = PROXY;
			} else if (line == "[stub]") {
				loadType = STUB;
			} else { // data
				if (loadType != UNKNOWN) {
					std::size_t pos = line.find('=');
					if (pos != line.npos) {
						std::string key = line.substr(0, pos);
						std::string value = line.substr(pos+1);

						if (loadType == DEFAULT) {
							if (key == "ipc") defaultIpc_ = value;
							else if (key == "folder") defaultFolder_ = value;
						} else if (loadType == PROXY) {
							libraries_[key][true] = value;
						} else {
							libraries_[key][false] = value;
						}
					}
				}
			}
		}
	} else {
		Logger::log("No configuration available. Using defaults.");
	}

	return false;
}

std::shared_ptr<Proxy>
Runtime::createProxy(
		const std::string &_domain, const std::string &_interface, const std::string &_instance,
		std::shared_ptr<MainLoopContext> _context) {

	std::string library = getLibrary(_domain, _interface, _instance, true);
	if (loadLibrary(library)) {
		for (auto factory : factories_) {
			std::shared_ptr<Proxy> proxy
				= factory.second->createProxy(_domain, _interface, _instance, _context);
			if (proxy)
				return proxy;
		}
	}

	return nullptr;
}

bool
Runtime::registerStub(const std::string &_domain, const std::string &_interface, const std::string &_instance,
				  	  std::shared_ptr<StubBase> _stub, std::shared_ptr<MainLoopContext> _context) {
	std::string library = getLibrary(_domain, _interface, _instance, false);
	if (loadLibrary(library)) {
		for (auto factory : factories_) {
			return factory.second->registerStub(_domain, _interface, _instance, _stub, _context);
		}
	}
	return false;
}

std::string
Runtime::getLibrary(
	const std::string &_domain, const std::string &_interface, const std::string &_instance,
	bool _isProxy) {

	std::string library;

	std::string address = _domain + ":" + _interface + ":" + _instance;
	auto libraryIterator = libraries_.find(address);
	if (libraryIterator != libraries_.end()) {
		auto addressIterator = libraryIterator->second.find(_isProxy);
		if (addressIterator != libraryIterator->second.end()) {
			library = addressIterator->second;
		}
	}

	// If no library was found, create default name
	if ("" == library) {
		library = "lib" + _domain + "__" + _interface + "__" + _instance;
		std::replace(library.begin(), library.end(), '.', '_');
	}

	return library;
}

bool
Runtime::loadLibrary(const std::string &_library) {
	std::string itsLibrary(_library);

	// TODO: decide whether this really is a good idea...
	if (itsLibrary.find(".so") != itsLibrary.length() - 3) {
		itsLibrary += ".so";
	}

	bool isLoaded(true);
	if (loadedLibraries_.end() == loadedLibraries_.find(itsLibrary)) {
		if (dlopen(itsLibrary.c_str(), RTLD_LAZY | RTLD_GLOBAL) != 0) {
			loadedLibraries_.insert(itsLibrary);
			Logger::log("Loading interface library \"", itsLibrary, "\" succeeded.");
		} else {
			Logger::log("Loading interface library \"", itsLibrary, "\" failed (", dlerror(), ")");
			isLoaded = false;
		}
	}
	return isLoaded;
}

} //Namespace CommonAPI
