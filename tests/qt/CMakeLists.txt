add_executable(hello hello.cpp)
target_link_libraries(hello PUBLIC Qt5::Core Qt5::Gui Qt5::Widgets)

add_executable(simple simple.cpp)
target_link_libraries(simple PUBLIC Qt5::Core Qt5::Gui Qt5::Widgets)

