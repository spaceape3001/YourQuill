add_executable(ask ask.cpp)
target_link_libraries(ask ${CURL_LIBRARIES})

add_executable(kvtree kvtree.cpp)
target_link_libraries(kvtree yq)

add_executable(metavalues metavalues.cpp)
target_link_libraries(metavalues yq yqmath)

add_executable(sqlquery sqlquery.cpp)
target_link_libraries(sqlquery yq)

add_executable(tcpsink tcpsink.cpp)

add_executable(vk_devices vk_devices.cpp)
target_link_libraries(vk_devices yqvulqan)

add_executable(vk_extensions vk_extensions.cpp)
target_link_libraries(vk_extensions yqvulqan)

add_executable(vk_layers vk_layers.cpp)
target_link_libraries(vk_layers yqvulqan)

add_executable(wkspinfo wkspinfo.cpp)
target_link_libraries(wkspinfo yq)

add_executable(xmlread xmlread.cpp)

