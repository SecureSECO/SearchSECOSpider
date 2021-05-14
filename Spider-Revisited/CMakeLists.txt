# This program has been developed by students from the bachelor Computer Science at
# Utrecht University within the Software Project course.
# © Copyright Utrecht University (Department of Information and Computing Sciences)


cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 20)

set(SPIDER_HEADER_LIST
   ExecuteCommand.h
   Filesystem.h
   Git.h
   GitSpider.h
   RunSpider.h
   Spider.h
   CodeBlock.h
   Error.h
   Logger.h
   loguru/loguru.hpp
)

add_library(spider
   ExecuteCommand.cpp
   Filesystem.cpp
   Git.cpp
   GitSpider.cpp
   RunSpider.cpp
   Spider.cpp
   Error.cpp
   Logger.cpp
   loguru/loguru.cpp
  ${SPIDER_HEADER_LIST}
)