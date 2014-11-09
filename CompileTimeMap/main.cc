/*
  Copyright (c) 2014, Spandan Sharma
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
      * Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
      * Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
      * The name of the contributor of this project may NOT be used to endorse
        or promote products derived from this software without specific prior
        written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL SPANDAN SHARMA BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/**************************************************************************************************
 * - ustulation@gmail.com
 *  ---------------
 * | Description:  |
 *  ---------------
 * Providing branching capabilities to the Compile-Time-Variadic Map posted in code project article:
 *      [ http://www.codeproject.com/Articles/598107/Compile-Time-Map ]
 * to enable differential treatment for the mapped and the unmapped data. The construct is widely
 * being used in more organizations now and this capability is now highly called for. This is a POC
 * for the same.
 **************************************************************************************************/

#include <string>

#include "compile_time_map.h"
#include "user_defined_types.h"

// ****************************************************************
// TEST: Type-to-Tag Mapping Functionality
// ****************************************************************
#define PRINT_TYPE_AND_FIND_TAG(Type) \
  Type {};\
  std::printf("Tag: %d\n", GivenTypeFindTag_v<Type>::value);\
  std::printf("-------------------------\n")

// ****************************************************************
// TEST: Tag-to-Type Mapping Functionality
// ****************************************************************
#define PRINT_TAG_AND_FIND_TYPE(tag) \
  std::printf("Tag: %d\n", tag);\
  GivenTagFindType_t<tag> {};\
  std::printf("-------------------------\n")

// ****************************************************************
// TEST: SFINAE Functionality for non-templated functions
// ****************************************************************
std::string IsTagged(const std::string& str_prefix) { return str_prefix + std::string {" tag value"}; }

// ****************************************************************
// TEST: SFINAE Functionality for templated functions
// ****************************************************************
struct CallableForTagged {
  template<typename T> using rem_ref_t = typename std::remove_reference<T>::type;
  template<typename T> using underlying_type_t = typename std::underlying_type<T>::type;

  template<typename Type, typename... Types>
  std::string operator()(const std::string& str_prefix, Type&&, Types&&...) {
    using ValueType_t = decltype(GivenTypeFindTag_v<rem_ref_t<Type>>::value);

    return str_prefix + std::string {" tag value "} +
    std::to_string(static_cast<underlying_type_t<ValueType_t>>(GivenTypeFindTag_v<rem_ref_t<Type>>::value));
  }
};

struct CallableForUntagged {
  template<typename... Types>
  std::string operator()(const std::string& str_prefix, Types&&...) {
    return str_prefix + std::string {" NO associated tag value"};
  }
};
// ****************************************************************
// ****************************************************************

int main() {
  std::printf("-------------------------\n");
  std::printf("-------------------------\n");
  std::printf("Testing Type-to-Tag Mapping Functionality\n");
  std::printf("-------------------------\n");

  PRINT_TYPE_AND_FIND_TAG(Two);
  PRINT_TYPE_AND_FIND_TAG(Three);
  PRINT_TYPE_AND_FIND_TAG(Five);
  // --------------------------------------------------------------------------
  // Hard-error: Intentional; to catch accessing un-mapped data at compile time
  // PRINT_TYPE_AND_FIND_TAG(Six);
  // --------------------------------------------------------------------------

  std::printf("-------------------------\n");
  std::printf("-------------------------\n");
  std::printf("Testing Tag-to-Type Mapping Functionality\n");
  std::printf("-------------------------\n");

  PRINT_TAG_AND_FIND_TYPE(Tag::e0);
  PRINT_TAG_AND_FIND_TYPE(Tag::e1);
  PRINT_TAG_AND_FIND_TYPE(Tag::e4);
  // --------------------------------------------------------------------------
  // Hard-error: Intentional; to catch accessing un-mapped data at compile time
  // PRINT_TAG_AND_FIND_TYPE(Tag::e6);
  // --------------------------------------------------------------------------

  std::printf("-------------------------\n");
  std::printf("-------------------------\n");
  std::printf("Testing SFINAE Functionality for non-templated functions and Lambdas\n");
  std::printf("[Note: Not Possible to use function templates as template arguments\n");
  std::printf("       Hence Not Possible to call passed function on arbitary object\n");
  std::printf("       without explicitly specifying type, which is ugly syntax]\n");
  std::printf("-------------------------\n");

  // ****************************************************************
  // TEST: SFINAE Functionality for non-generic (C++11) lambdas
  // ****************************************************************
  auto IsUntagged = [](const std::string& str_prefix) {
    return str_prefix + std::string {" NO associated tag value"};
  };

  Seven {};
  std::printf("%s\n",
              ConditionallyExecute<Seven>::Execute(IsTagged, IsUntagged, "Given type has").c_str());
  std::printf("-------------------------\n");

  Five {};
  std::printf("%s\n",
              ConditionallyExecute<Five>::Execute(IsTagged, IsUntagged, "Given type has").c_str());
  std::printf("-------------------------\n");

  std::printf("-------------------------\n");
  std::printf("-------------------------\n");
  std::printf("Testing SFINAE Functionality for templated functions\n");
  std::printf("[Note: After wrapping operator() template in a struct it is possible to\n");
  std::printf("       call passed functor on arbitary parameters without explicitly\n");
  std::printf("       specifying type. Now we can selectively print the tag for those it\n");
  std::printf("       is available. Generic lambdas of C++14 will solve this too]\n");
  std::printf("-------------------------\n");

  // --------------------------------------------------------------------------
  // Hard-error: Intentional; to catch invalid call after SFINAE dispatch at
  //             compile time. SFINAE dispatch chooses WorkIfTagged for which
  //             the following will fail.
  // Three {};
  // std::printf("%s\n",
  //             ConditionallyExecute<Three>::Execute(WorkIfTagged {},
  //                                                  WorkIfNotTagged {},
  //                                                  "Given type has").c_str());
  // std::printf("-------------------------\n");
  // --------------------------------------------------------------------------

  Six {};
  std::printf("%s\n",
              ConditionallyExecute<Six>::Execute(CallableForTagged {},
                                                 CallableForUntagged {},
                                                 "Given type has").c_str());
  std::printf("-------------------------\n");

  std::printf("%s\n",
              ConditionallyExecute<Four>::Execute(CallableForTagged {},
                                                  CallableForUntagged {},
                                                  "Given type has",
                                                  Four {}).c_str());
  std::printf("-------------------------\n");

  std::printf("%s\n",
              ConditionallyExecute<Seven>::Execute(CallableForTagged {},
                                                   CallableForUntagged {},
                                                   "Given type has",
                                                   Seven {}).c_str());
  std::printf("-------------------------\n");

  std::printf("%s\n",
              ConditionallyExecute<Zero>::Execute(CallableForTagged {},
                                                  CallableForUntagged {},
                                                  "Given type has",
                                                  Zero {}).c_str());
  std::printf("-------------------------\n");
}
