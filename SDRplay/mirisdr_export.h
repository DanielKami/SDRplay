 

#ifndef __SDR_EXPORT_H
#define __SDR_EXPORT_H

 
#if COMPILING __SDR_EXPORT_H //BUILDING_DLL
#    define __SDR_EXPORT     __declspec(dllexport)
#else
#    define __SDR_IMPORT     __declspec(dllimport)
#  endif
 
 
 

//#ifndef SDR_STATIC
//#  ifdef SDR_EXPORTS
//#    define SDRPLAY_API __SDR_EXPORT
//#  else
//#    define SDRPLAY_API __SDR_IMPORT
//#  endif
//#else
//#  define MIRISDR_API
//#endif
#endif /* __SDR_EXPORT_H */
