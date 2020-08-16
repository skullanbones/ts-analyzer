
#ifndef MPEG2TS_EXPORT_H
#define MPEG2TS_EXPORT_H

#ifdef MPEG2TS_STATIC_DEFINE
#  define MPEG2TS_EXPORT
#  define MPEG2TS_NO_EXPORT
#else
#  ifndef MPEG2TS_EXPORT
#    ifdef mpeg2ts_EXPORTS
        /* We are building this library */
#      define MPEG2TS_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define MPEG2TS_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef MPEG2TS_NO_EXPORT
#    define MPEG2TS_NO_EXPORT 
#  endif
#endif

#ifndef MPEG2TS_DEPRECATED
#  define MPEG2TS_DEPRECATED __declspec(deprecated)
#endif

#ifndef MPEG2TS_DEPRECATED_EXPORT
#  define MPEG2TS_DEPRECATED_EXPORT MPEG2TS_EXPORT MPEG2TS_DEPRECATED
#endif

#ifndef MPEG2TS_DEPRECATED_NO_EXPORT
#  define MPEG2TS_DEPRECATED_NO_EXPORT MPEG2TS_NO_EXPORT MPEG2TS_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef MPEG2TS_NO_DEPRECATED
#    define MPEG2TS_NO_DEPRECATED
#  endif
#endif

#endif /* MPEG2TS_EXPORT_H */
