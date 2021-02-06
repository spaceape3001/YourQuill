find_path(nghttp2_INCLUDE_DIRS nghttp2.h PATH_SUFFIXES nghttp2)
set(nghttp2_INCLUDE_DIRS ${nghttp2_INCLUDE_DIRS}/../)
include(SelectLibraryConfigurations)
find_library(nghttp2_LIBRARY_RELEASE NAMES nghttp2)
find_library(nghttp2_LIBRARY_DEBUG NAMES nghttp2 HINTS debug)
select_library_configurations(nghttp2)

find_library(nghttp2_asio_LIBRARY_RELEASE NAMES nghttp2_asio)
find_library(nghttp2_asio_LIBRARY_DEBUG NAMES nghttp2_asio HINTS debug)
select_library_configurations(nghttp2_asio)

