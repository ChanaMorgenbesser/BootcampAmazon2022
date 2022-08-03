#ifndef GAZ_CAM_LIB_DLL_GLOBAL_H
#define GAZ_CAM_LIB_DLL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(GAZ_CAM_LIB_DLL_LIBRARY)
#  define GAZ_CAM_LIB_DLL_EXPORT Q_DECL_EXPORT
#else
#  define GAZ_CAM_LIB_DLL_EXPORT Q_DECL_IMPORT
#endif

#endif // GAZ_CAM_LIB_DLL_GLOBAL_H
