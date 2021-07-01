# This program has been developed by students from the bachelor Computer Science at
# Utrecht University within the Software Project course.
# © Copyright Utrecht University (Department of Information and Computing Sciences)

cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 20)

message("Building libSearchSECOSpider...")
if(NOT DEFINED LOGURU_INCLUDED)
   set(LOGURU_FILES
   loguru/loguru.hpp
   loguru/loguru.cpp
   )
   set(LOGURU_INCLUDED true PARENT_SCOPE)
   message("Including loguru files...")
endif()

set(SPIDER_HEADER_LIST
   ExecuteCommand.h
   Filesystem.h
   Git.h
   GitSpider.h
   RunSpider.h
   Spider.h
   CodeBlock.h
   ErrorSpider.h
   Logger.h
)

add_library(libSearchSECOSpider
   ExecuteCommand.cpp
   Filesystem.cpp
   Git.cpp
   GitSpider.cpp
   RunSpider.cpp
   Spider.cpp
   ErrorSpider.cpp
   Logger.cpp
  ${SPIDER_HEADER_LIST}
  ${LOGURU_FILES}
)

if(CMAKE_DL_LIBS)
    target_link_libraries(libSearchSECOSpider ${CMAKE_DL_LIBS})
endif()
