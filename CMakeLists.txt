###################################################################################################
#  Copyright (c) 2014, Spandan Sharma
#  All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions are met:
#      * Redistributions of source code must retain the above copyright
#        notice, this list of conditions and the following disclaimer.
#      * Redistributions in binary form must reproduce the above copyright
#        notice, this list of conditions and the following disclaimer in the
#        documentation and/or other materials provided with the distribution.
#      * The name of the contributor of this project may NOT be used to endorse
#        or promote products derived from this software without specific prior
#        written permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
#  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
#  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL SPANDAN SHARMA BE LIABLE FOR ANY
#  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
#  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
#  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
#  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
###################################################################################################

###################################################################################################
# - ustulation@gmail.com
#  ---------------
# | Description:  |
#  ---------------
# Top-Level CmakeLists.
###################################################################################################

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

string(TOLOWER "${CMAKE_BUILD_TYPE}" BuildConfig)
if("${BuildConfig}" STREQUAL "debug")
  message(" ----------------------------------------------")
  message("| Running in Debug mode turns VERBOSE mode On. |")
  message("| Try running in Release unless Debugging is   |")
  message("| required.                                    |")
  message(" ----------------------------------------------")
  set(CMAKE_VERBOSE_MAKEFILE ON)
endif()

set(SubModules
  CompileTimeMapBaseConcept
  CompileTimeMapWithGenericEnumKey
  CompileTimeMapWithGenericNonTypeKey
)

foreach(SubModule ${SubModules})
  add_subdirectory(./${SubModule})
endforeach()

add_custom_target(ExcecuteCompileTimeMapAllTargets
  COMMAND echo "============================================="
  COMMAND echo "============================================="
  COMMAND echo "      Project CompileTimeMapBaseConcept"
  COMMAND echo "============================================="
  COMMAND $<TARGET_FILE:CompileTimeMapBaseConcept>
  COMMAND echo "============================================="
  COMMAND echo "============================================="
  COMMAND echo "   Project CompileTimeMapWithGenericEnumKey"
  COMMAND echo "============================================="
  COMMAND $<TARGET_FILE:CompileTimeMapWithGenericEnumKey>
  COMMAND echo "============================================="
  COMMAND echo "============================================="
  COMMAND echo " Project CompileTimeMapWithGenericNonTypeKey"
  COMMAND echo "============================================="
  COMMAND $<TARGET_FILE:CompileTimeMapWithGenericNonTypeKey>
  COMMAND echo "============================================="
)
