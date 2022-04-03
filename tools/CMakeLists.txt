add_executable(ask ask.cpp)
target_link_libraries(ask ${CURL_LIBRARIES})

add_executable(kvtree kvtree.cpp)
target_link_libraries(kvtree yq)

add_executable(metavalues metavalues.cpp)
target_link_libraries(metavalues yq)

add_executable(sqlquery sqlquery.cpp)
target_link_libraries(sqlquery yq)

add_executable(tcpsink tcpsink.cpp)

add_executable(wkspinfo wkspinfo.cpp)
target_link_libraries(wkspinfo yq)

add_executable(xmlread xmlread.cpp)

# add_subdirectory(importer)
# add_subdirectory(attrx)
# add_subdirectory(convert)
# add_subdirectory(reformat)

