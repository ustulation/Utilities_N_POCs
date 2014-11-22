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
# Providing branching capabilities to the Compile-Time-Variadic Map posted in code project article:
#      [ http://www.codeproject.com/Articles/598107/Compile-Time-Map ]
# to enable differential treatment for the mapped and the unmapped data. The construct is widely
# being used in more organizations now and this capability is now highly called for. This is a POC
# for the same.
###################################################################################################

project(CompileTimeMapBaseConcept)

file(GLOB AllCodeFiles "*.cc" "*.h")

add_executable(${PROJECT_NAME} ${AllCodeFiles})
target_compile_options(${PROJECT_NAME}
PRIVATE
$<$<BOOL:${UNIX}>:
  -std=c++11
  -Wall
  -Werror
  -O3
>
)
