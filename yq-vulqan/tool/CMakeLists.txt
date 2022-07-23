################################################################################
##
##  YOUR QUILL
##
################################################################################


add_executable(glfw_joysticks glfw_joysticks.cpp)
target_link_libraries(glfw_joysticks yqengine)

add_executable(glfw_monitors glfw_monitors.cpp)
target_link_libraries(glfw_monitors yqengine)

add_executable(vk_devices vk_devices.cpp)
target_link_libraries(vk_devices yqengine)

add_executable(vk_extensions vk_extensions.cpp)
target_link_libraries(vk_extensions yqengine)

add_executable(vk_layers vk_layers.cpp)
target_link_libraries(vk_layers yqengine)


