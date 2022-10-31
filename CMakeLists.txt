cmake_minimum_required(VERSION 3.10)

# cmake -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang -DCMAKE_BUILD_TYPE=Release -S tplubi -B tplubi

set (PROJECT_NAME sdl_2dgame)

project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (THE_APP_PROJECTS_OUTPUT ${CMAKE_CURRENT_SOURCE_DIR})
set( SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src )
set( INC_DIR $ENV{DEVLIB_INCLUDE} )



#this will force all output under the folder of the curren  t project
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${THE_APP_PROJECTS_OUTPUT}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${THE_APP_PROJECTS_OUTPUT}/lib )



set (AVX2SOURCE   ${SRC_DIR}/assetmanager.cpp
                  ${SRC_DIR}/collision.cpp
                  ${SRC_DIR}/entity.cpp
                  ${SRC_DIR}/entitymanager.cpp
                  ${SRC_DIR}/fontmanager.cpp
                  ${SRC_DIR}/texturemanager.cpp
                  ${SRC_DIR}/main.cpp
                  ${SRC_DIR}/map.cpp
                  ${SRC_DIR}/game.cpp
                  )

set (AVX2HEADER ${SRC_DIR}/animation.hpp
                ${SRC_DIR}/assetmanager.hpp
                ${SRC_DIR}/collision.hpp
                ${SRC_DIR}/component.hpp
                ${SRC_DIR}/constants.hpp
                ${SRC_DIR}/entity.hpp
                ${SRC_DIR}/entitymanager.hpp
                ${SRC_DIR}/fontmanager.hpp
                ${SRC_DIR}/game.hpp
                ${SRC_DIR}/graphicshdr.hpp
                ${SRC_DIR}/map.hpp
                ${SRC_DIR}/texturemanager.hpp
                ${SRC_DIR}/components/colliderComponent.hpp
                ${SRC_DIR}/components/keyboardcontrol.hpp
                ${SRC_DIR}/components/projectileemiterComponent.hpp
                ${SRC_DIR}/components/spritecomponent.hpp
                ${SRC_DIR}/components/textlabelComponent.hpp
                ${SRC_DIR}/components/tilecomponent.hpp
                ${SRC_DIR}/components/transformComponent.hpp
)

add_executable(${PROJECT_NAME}  ${AVX2SOURCE}  ${AVX2HEADER} )

target_include_directories( ${PROJECT_NAME}
                            PUBLIC     
                                ${INC_DIR}
                          )

# define 
target_compile_definitions(  ${PROJECT_NAME} PUBLIC  _USE_MATH_DEFINES )                          

target_compile_options(${PROJECT_NAME} 
                               PUBLIC
                                 $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Release>>:/MT /W4 /O2 /std:c++17 /wd4996 /wd4127  /wd5105 /wd5104 /NODEFAULTLIB >
                                 $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Debug>>:/MD /Od /Zi /std:c++17 /wd5105 /wd5104 >
                                 $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>: -Wall -Wextra -Wfloat-conversion>
                                 $<$<AND:$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>,$<CONFIG:Release>>: -std=c++17 -mavx2  -mfma -O3 -DNDEBUG > 
                                 $<$<AND:$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>,$<CONFIG:Debug>>:-g -mavx2  -mfma >                         
                               )
         
         
set_target_properties (${PROJECT_NAME} 
                        PROPERTIES 
                              DEBUG_POSTFIX "-d"
                      )

         
target_compile_definitions(${PROJECT_NAME}
                          PUBLIC
                          $<$<BOOL:${WIN32}>:_WIN32_WINNT=0x0601>
                          $<$<BOOL:${WIN32}>:_UNICODE>
                          $<$<BOOL:${WIN32}>:UNICODE>
                          $<$<BOOL:${WIN32}>:_CRT_SECURE_NO_WARNINGS>
                          $<$<BOOL:${WIN32}>:NOMINMAX>                        
                        )

 

target_link_directories( ${PROJECT_NAME}  
                        PUBLIC  
                          $ENV{DEVLIB_LIB}  
                      )

target_link_libraries( ${PROJECT_NAME}
  $<$<BOOL:${WIN32}>:Shlwapi>
  $<$<BOOL:${WIN32}>:lua>
  $<$<BOOL:${WIN32}>:SDL2>
  $<$<BOOL:${WIN32}>:SDL2_image>
  $<$<BOOL:${WIN32}>:SDL2_ttf>
)