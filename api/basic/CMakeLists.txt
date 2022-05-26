#   This is a testing file, excluded from GIT, used for quick test compilation 
#   of code before sliding it into the appropriate spot of the build.
#   (ie.  prototyping goes here, but do not commit/push test.cpp.)
if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/test.cpp)
    set(basic_test_source ./test.cpp)
endif()

set(basic_sources
    ${basic_test_source}
    
    ./Array1.hpp
    ./Array2.hpp
    ./ByteArray.hpp
    ./CmdArgs.cpp
    ./CmdArgs.hpp
    ./Comma.hpp
    ./Compare.hpp
    ./Concat.hpp
    ./DbgRel.hpp
    ./Deque.hpp
    ./DelayInit.cpp
    ./DelayInit.hpp
    ./DiffEngine.hpp
    ./Enum.cpp
    ./Enum.hpp
    ./EnumMap.hpp
    ./Execute.cpp
    ./Execute.hpp
    ./Guarded.hpp
    ./FixedBuffer.hpp
    ./Flag.cpp
    ./Flag.hpp
    ./Hash.hpp
    ./IgCase.hpp
    ./IgCase.cpp
    ./IntRange.hpp
    ./Iter32.hpp
    ./IterW.hpp
    ./List.hpp
    ./LogFwd.hpp
    ./Logging.cpp
    ./Logging.hpp
    ./Map.hpp
    ./MultiMap.hpp
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
    ./Stack.hpp
    ./Stream.cpp
    ./Stream.hpp
    ./ThreadId.cpp
    ./ThreadId.hpp
    ./Touched.hpp
    ./Vector.hpp
    ./Version.hpp
    
    ./c_utils.hpp
    ./text_utils.cpp
    ./text_utils.hpp

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

IF(GCC)
    SET(GCC_FSLIB stdc++fs)
ENDIF()

target_link_libraries(yqbasic 
    PUBLIC
        ${LOG4CPP_LIBRARIES} 
        ${TBB_LIBRARIES}
        ${GCC_FSLIB}
)

LinkTest(yqbasic)

