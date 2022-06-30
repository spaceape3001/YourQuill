################################################################################
##
##  YOUR QUILL
##
################################################################################

#   This is a testing file, excluded from GIT, used for quick test compilation 
#   of code before sliding it into the appropriate spot of the build.
#   (ie.  prototyping goes here, but do not commit/push test.cpp.)
if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/test.cpp)
    set(basic_test_source ./test.cpp)
endif()

set(basic_sources
    ${basic_test_source}
    
    ./basic.cpp
    
    ./AbstractFile.ipp
    ./AbstractFile.hpp
    ./Any.ipp
    ./Any.hpp
    ./Array1.hpp
    ./Array2.hpp
    ./BasicApp.ipp
    ./BasicApp.hpp
    ./BasicBuffer.hpp
    ./ByteArray.hpp
    ./CollectionUtils.hpp
    ./Comma.hpp
    ./Compare.hpp
    ./Concat.hpp
    ./ContentType.ipp
    ./ContentType.hpp
    ./Curl.ipp
    ./Curl.hpp
    ./DbgRel.hpp
    ./Deque.hpp
    ./DelayInit.ipp
    ./DelayInit.hpp
    ./DiffEngine.hpp
    ./DirUtils.ipp
    ./DirUtils.hpp
    ./Encode64.ipp
    ./Encode64.hpp
    ./Enum.ipp
    ./Enum.hpp
    ./EnumMap.hpp
    ./Execute.ipp
    ./Execute.hpp
    ./Guarded.hpp
    ./FileUtils.ipp
    ./FileUtils.hpp
    ./FixedBuffer.hpp
    ./Flag.ipp
    ./Flag.hpp
    ./Format.ipp
    ./Format.hpp
    ./Hash.hpp
    ./Http.hpp
    ./HttpOp.hpp
    ./HttpParser.ipp
    ./HttpParser.hpp
    ./HttpStatus.ipp
    ./HttpStatus.hpp
    ./IgCase.hpp
    ./IgCase.ipp
    ./IntRange.hpp
    ./Iter32.hpp
    ./Iter32.ipp
    ./IterW.hpp
    ./KV.hpp
    ./List.hpp
    ./LogFwd.hpp
    ./Logging.ipp
    ./Logging.hpp
    ./Map.hpp
    ./MultiMap.hpp
    ./Object.ipp
    ./Object.hpp
    ./PidFile.ipp
    ./PidFile.hpp
    ./PluginLoader.ipp
    ./PluginLoader.hpp
    ./Random.ipp
    ./Random.hpp
    ./Ref.ipp
    ./Ref.hpp
    ./Result.hpp
    ./RevIgCase.hpp
    ./RevIgCase.ipp
    ./Safety.hpp
    ./Set.hpp
    ./Span.hpp
    ./SqlError.ipp
    ./SqlError.hpp
    ./SqlLite.ipp
    ./SqlLite.hpp
    ./SqlLogging.hpp
    ./SqlQuery.ipp
    ./SqlQuery.hpp
    ./SqlUtils.ipp
    ./SqlUtils.hpp
    ./Stack.hpp
    ./Stream.ipp
    ./Stream.hpp
    ./StreamOps.ipp
    ./StreamOps.hpp
    ./TextUtils.ipp
    ./TextUtils.hpp
    ./ThreadId.ipp
    ./ThreadId.hpp
    ./Touched.hpp
    ./Url.ipp
    ./Url.hpp
    ./Vector.hpp
    ./Version.hpp
    ./VersionSpec.hpp
    ./VersionSpec.ipp
    ./XmlFile.ipp
    ./XmlFile.hpp
    ./XmlObject.ipp
    ./XmlObject.hpp
    ./XmlSerialize.hpp
    ./XmlUtils.ipp
    ./XmlUtils.hpp
    
    ./meta/AnyDef.hpp
    ./meta/AnyImpl.hpp
    ./meta/ArgInfo.ipp
    ./meta/ArgInfo.hpp
    ./meta/CompoundInfo.ipp
    ./meta/CompoundInfo.hpp
    ./meta/CompoundInfoDynamic.hpp
    ./meta/CompoundInfoStatic.hpp
    ./meta/DataBlock.hpp
    ./meta/DynamicPropGetter.hpp
    ./meta/DynamicPropSetter.hpp
    ./meta/Global.ipp
    ./meta/Global.hpp
    ./meta/GlobalInfoWriter.hpp
    ./meta/InfoBinder.hpp
    ./meta/Init.hpp
    ./meta/Meta.ipp
    ./meta/Meta.hpp
    ./meta/MetaRepo.ipp
    ./meta/MetaRepo.hpp
    ./meta/MetaWriter.ipp
    ./meta/MetaWriter.hpp
    ./meta/MethodInfo.ipp
    ./meta/MethodInfo.hpp
    ./meta/MethodInfoWriter.hpp
    ./meta/ObjectInfo.ipp
    ./meta/ObjectInfo.hpp
    ./meta/ObjectInfoWriter.hpp
    ./meta/PropGetter.ipp
    ./meta/PropGetter.hpp
    ./meta/PropSetter.ipp
    ./meta/PropSetter.hpp
    ./meta/PropertyInfo.ipp
    ./meta/PropertyInfo.hpp
    ./meta/PropertyInfoWriter.hpp
    ./meta/ReservedIDs.hpp
    ./meta/StaticPropGetter.hpp
    ./meta/StaticPropSetter.hpp
    ./meta/StdTypes.ipp
    ./meta/TypeInfo.ipp
    ./meta/TypeInfo.hpp
    ./meta/TypeInfoWriter.hpp

    ./stream/Bytes.ipp
    ./stream/Bytes.hpp
    ./stream/First.ipp
    ./stream/First.hpp
    ./stream/File.ipp
    ./stream/File.hpp
    ./stream/LineCol.ipp
    ./stream/LineCol.hpp
    ./stream/OSFile.ipp
    ./stream/OSFile.hpp
    ./stream/Text.hpp
    ./stream/Text.ipp

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

