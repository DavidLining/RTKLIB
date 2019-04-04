cmake_minimum_required(VERSION 2.8.11)

set(project_name rtkrcv)
project(${project_name})

set(fake_target_name ${project_name})

add_custom_target(${fake_target_name} ALL 
    COMMAND echo "build ${fake_target_name}")

add_custom_command(TARGET ${fake_target_name} POST_BUILD
        COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}* ${CMAKE_INSTALL_PREFIX}/usr/local/bin/
        COMMENT "${project_name} copy bins and config")

