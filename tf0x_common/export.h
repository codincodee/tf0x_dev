#ifndef TF02_DRIVER_EXPORT_H
#define TF02_DRIVER_EXPORT_H

#ifdef _WIN32
#	ifdef tf0x_common_EXPORTS
#		define API __declspec(dllexport)
#	else
#		define API __declspec(dllimport)
#	endif
#else
#	define API
#endif // WIN32

#endif // TF02_DRIVER_EXPORT_H
