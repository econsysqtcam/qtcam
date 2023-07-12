#ifndef COMMON_H
#define COMMON_H

#define LAUNCHPAD 0 // by Default, this variable is set as 1.
		     // when the source is taken from econ git hub and build manually, This variable needs to be set as 0.


/* checking version compatibility */
#define LIBAVCODEC_VER_AT_LEAST(major,minor)  (LIBAVCODEC_VERSION_MAJOR > major || \
                                              (LIBAVCODEC_VERSION_MAJOR == major && \
                                               LIBAVCODEC_VERSION_MINOR >= minor))


#define LIBAVUTIL_VER_AT_LEAST(major,minor)  (LIBAVUTIL_VERSION_MAJOR > major || \
                                              (LIBAVUTIL_VERSION_MAJOR == major && \
                                               LIBAVUTIL_VERSION_MINOR >= minor))

#endif // COMMON_H
