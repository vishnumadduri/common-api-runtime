#ifndef COMMONAPI_DLLEXPORTMACRO_HPP_
#define COMMONAPI_DLLEXPORTMACRO_HPP_

#ifdef WIN32
#define DllExport __declspec(dllexport)
#else
#define DllExport
#endif

#endif