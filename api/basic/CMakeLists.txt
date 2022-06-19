#   This is a testing file, excluded from GIT, used for quick test compilation 
#   of code before sliding it into the appropriate spot of the build.
#   (ie.  prototyping goes here, but do not commit/push test.cpp.)
if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/test.cpp)
    set(basic_test_source ./test.cpp)
endif()

set(basic_sources
    ${basic_test_source}
    
    ./AbstractFile.cpp
    ./AbstractFile.hpp
    ./Any.cpp
    ./Any.hpp
    ./Array1.hpp
    ./Array2.hpp
    ./BasicApp.cpp
    ./BasicApp.hpp
    ./BasicBuffer.hpp
    ./ByteArray.hpp
    ./CollectionUtils.hpp
    ./Comma.hpp
    ./Compare.hpp
    ./Concat.hpp
    ./ContentType.cpp
    ./ContentType.hpp
    ./Curl.cpp
    ./Curl.hpp
    ./DbgRel.hpp
    ./Deque.hpp
    ./DelayInit.cpp
    ./DelayInit.hpp
    ./DiffEngine.hpp
    ./DirUtils.cpp
    ./DirUtils.hpp
    ./Encode64.cpp
    ./Encode64.hpp
    ./Enum.cpp
    ./Enum.hpp
    ./EnumMap.hpp
    ./Execute.cpp
    ./Execute.hpp
    ./Guarded.hpp
    ./FileUtils.cpp
    ./FileUtils.hpp
    ./FixedBuffer.hpp
    ./Flag.cpp
    ./Flag.hpp
    ./Format.cpp
    ./Format.hpp
    ./Hash.hpp
    ./Http.hpp
    ./HttpOp.hpp
    ./HttpParser.cpp
    ./HttpParser.hpp
    ./HttpStatus.cpp
    ./HttpStatus.hpp
    ./IgCase.hpp
    ./IgCase.cpp
    ./IntRange.hpp
    ./Iter32.hpp
    ./IterW.hpp
    ./KV.hpp
    ./List.hpp
    ./LogFwd.hpp
    ./Logging.cpp
    ./Logging.hpp
    ./Map.hpp
    ./MultiMap.hpp
    ./Object.cpp
    ./Object.hpp
    ./PidFile.cpp
    ./PidFile.hpp
    ./PluginLoader.cpp
    ./PluginLoader.hpp
    ./Random.cpp
    ./Random.hpp
    ./Ref.cpp
    ./Ref.hpp
    ./Result.hpp
    ./RevIgCase.hpp
    ./RevIgCase.cpp
    ./Safety.hpp
    ./Set.hpp
    ./Span.hpp
    ./SqlError.cpp
    ./SqlError.hpp
    ./SqlLite.cpp
    ./SqlLite.hpp
    ./SqlLogging.hpp
    ./SqlQuery.cpp
    ./SqlQuery.hpp
    ./SqlUtils.cpp
    ./SqlUtils.hpp
    ./Stack.hpp
    ./Stream.cpp
    ./Stream.hpp
    ./StreamOps.cpp
    ./StreamOps.hpp
    ./TextUtils.cpp
    ./TextUtils.hpp
    ./ThreadId.cpp
    ./ThreadId.hpp
    ./Touched.hpp
    ./Url.cpp
    ./Url.hpp
    ./Vector.hpp
    ./Version.hpp
    ./VersionSpec.hpp
    ./VersionSpec.cpp
    ./XmlFile.cpp
    ./XmlFile.hpp
    ./XmlObject.cpp
    ./XmlObject.hpp
    ./XmlSerialize.hpp
    ./XmlUtils.cpp
    ./XmlUtils.hpp
    
    ./meta/AnyDef.hpp
    ./meta/AnyImpl.hpp
    ./meta/ArgInfo.cpp
    ./meta/ArgInfo.hpp
    ./meta/CompoundInfo.cpp
    ./meta/CompoundInfo.hpp
    ./meta/CompoundInfoDynamic.hpp
    ./meta/CompoundInfoStatic.hpp
    ./meta/DataBlock.hpp
    ./meta/DynamicPropGetter.hpp
    ./meta/DynamicPropSetter.hpp
    ./meta/Global.cpp
    ./meta/Global.hpp
    ./meta/GlobalInfoWriter.hpp
    ./meta/InfoBinder.hpp
    ./meta/Init.hpp
    ./meta/Meta.cpp
    ./meta/Meta.hpp
    ./meta/MetaRepo.cpp
    ./meta/MetaRepo.hpp
    ./meta/MetaWriter.cpp
    ./meta/MetaWriter.hpp
    ./meta/MethodInfo.cpp
    ./meta/MethodInfo.hpp
    ./meta/MethodInfoWriter.hpp
    ./meta/ObjectInfo.cpp
    ./meta/ObjectInfo.hpp
    ./meta/ObjectInfoWriter.hpp
    ./meta/PropGetter.cpp
    ./meta/PropGetter.hpp
    ./meta/PropSetter.cpp
    ./meta/PropSetter.hpp
    ./meta/PropertyInfo.cpp
    ./meta/PropertyInfo.hpp
    ./meta/PropertyInfoWriter.hpp
    ./meta/ReservedIDs.hpp
    ./meta/StaticPropGetter.hpp
    ./meta/StaticPropSetter.hpp
    ./meta/StdTypes.cpp
    ./meta/TypeInfo.cpp
    ./meta/TypeInfo.hpp
    ./meta/TypeInfoWriter.hpp

    ./stream/Bytes.cpp
    ./stream/Bytes.hpp
    ./stream/First.cpp
    ./stream/First.hpp
    ./stream/File.cpp
    ./stream/File.hpp
    ./stream/LineCol.cpp
    ./stream/LineCol.hpp
    ./stream/OSFile.cpp
    ./stream/OSFile.hpp
    ./stream/Text.hpp
    ./stream/Text.cpp

    ./trait/always_false.hpp
    ./trait/always_true.hpp
    ./trait/can_two_add.hpp
    ./trait/can_two_divide.hpp
    ./trait/can_two_multiply.hpp
    ./trait/can_two_subtract.hpp
    ./trait/has_addition.hpp
    ./trait/has_equality.hpp
    ./trait/has_inequality.hpp
    ./trait/has_less.hpp
    ./trait/has_subtraction.hpp
    ./trait/is_template.hpp
    ./trait/not_copyable.hpp
    ./trait/not_moveable.hpp
    ./trait/sink.hpp


)

add_library(yqbasic SHARED ${basic_sources})

target_compile_definitions(yqbasic PRIVATE 
    YQ__CORE__OVERRIDE 
)

IF(GCC)
    SET(GCC_FSLIB stdc++fs)
ENDIF()

target_link_libraries(yqbasic 
    PUBLIC
        ${LOG4CPP_LIBRARIES} 
        ${TBB_LIBRARIES}
        ${CURL_LIBRARIES}
        ${GCC_FSLIB}
        ${SQLite3_LIBRARY}
)

LinkTest(yqbasic)

