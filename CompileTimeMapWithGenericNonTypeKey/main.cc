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

#include <sstream>

#include "compile_time_map.h"
#include "user_defined_types.h"

// ****************************************************************
// TEST: Type-to-Tag Mapping Functionality
// ****************************************************************
#define PRINT_TYPE_AND_FIND_TAG(Mapper, Type) \
  Type {};\
  std::cerr << "Tag: " << *Mapper::GivenTypeFindTag_v<Type>::value << '\n';\
  std::cerr << "-------------------------\n";

// ****************************************************************
// TEST: Tag-to-Type Mapping Functionality
// ****************************************************************
#define PRINT_TAG_AND_FIND_TYPE(Mapper, tag) \
  std::cerr << "Tag: " << tag << '\n';\
  Mapper::GivenTagFindType_t<&tag> {};\
  std::cerr << "-------------------------\n";

// ****************************************************************
// TEST: SFINAE Functionality for non-templated functions
// ****************************************************************
std::string IsTagged(const std::string& str_prefix) { return str_prefix + std::string {" tag value"}; }

// ****************************************************************
// TEST: SFINAE Functionality for templated functions
// ****************************************************************
template<typename Mapper>
struct CallableForTagged {
  template<typename T> using rem_ref_t = typename std::remove_reference<T>::type;

  template<typename Type, typename... Types>
  std::string operator()(const std::string& str_prefix, Type&&, Types&&...) {
    std::stringstream str_stream_;
    str_stream_ << *Mapper::template GivenTypeFindTag_v<rem_ref_t<Type>>::value;
    return str_prefix + std::string {" tag value "} + str_stream_.str();
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
  using CTM_sz = CTM<
  PairSz_t<&s0, Zero>, PairSz_t<&s1, One>, PairSz_t<&s2, Two>, PairSz_t<&s3, Three>,
  PairSz_t<&s4, Four>, PairSz_t<&s5, Five>
  >;

  using CTM_db = CTM<
  PairDb_t<&d0, Zero>, PairDb_t<&d1, One>, PairDb_t<&d2, Two>, PairDb_t<&d3, Three>,
  PairDb_t<&d4, Four>, PairDb_t<&d5, Five>
  >;

  std::cerr << "-------------------------\n";
  std::cerr << "-------------------------\n";
  std::cerr << "Testing Type-to-Tag Mapping Functionality\n";
  std::cerr << "-------------------------\n";

  std::cerr << "#### MAP String ####\n";

  PRINT_TYPE_AND_FIND_TAG(CTM_sz, Two);
  PRINT_TYPE_AND_FIND_TAG(CTM_sz, Three);
  PRINT_TYPE_AND_FIND_TAG(CTM_sz, Five);
  // --------------------------------------------------------------------------
  // Hard-error: Intentional; to catch accessing un-mapped data at compile time
  // PRINT_TYPE_AND_FIND_TAG(Six);
  // --------------------------------------------------------------------------

  std::cerr << "#### MAP Double ####\n";

  PRINT_TYPE_AND_FIND_TAG(CTM_db, Two);
  PRINT_TYPE_AND_FIND_TAG(CTM_db, Three);
  PRINT_TYPE_AND_FIND_TAG(CTM_db, Five);
  // --------------------------------------------------------------------------
  // Hard-error: Intentional; to catch accessing un-mapped data at compile time
  // PRINT_TYPE_AND_FIND_TAG(Six);
  // --------------------------------------------------------------------------

  std::cerr << "-------------------------\n";
  std::cerr << "-------------------------\n";
  std::cerr << "Testing Tag-to-Type Mapping Functionality\n";
  std::cerr << "-------------------------\n";

  std::cerr << "#### MAP String ####\n";

  PRINT_TAG_AND_FIND_TYPE(CTM_sz, s0);
  PRINT_TAG_AND_FIND_TYPE(CTM_sz, s1);
  PRINT_TAG_AND_FIND_TYPE(CTM_sz, s4);
  // --------------------------------------------------------------------------
  // Hard-error: Intentional; to catch accessing un-mapped data at compile time
  // PRINT_TAG_AND_FIND_TYPE(CTM_sz, s6);
  // --------------------------------------------------------------------------

  std::cerr << "#### MAP Double ####\n";

  PRINT_TAG_AND_FIND_TYPE(CTM_db, d0);
  PRINT_TAG_AND_FIND_TYPE(CTM_db, d1);
  PRINT_TAG_AND_FIND_TYPE(CTM_db, d4);
  // --------------------------------------------------------------------------
  // Hard-error: Intentional; to catch accessing un-mapped data at compile time
  // PRINT_TAG_AND_FIND_TYPE(CTM_db, d6);
  // --------------------------------------------------------------------------

  std::cerr << "-------------------------\n";
  std::cerr << "-------------------------\n";
  std::cerr << "Testing SFINAE Functionality for non-templated functions and Lambdas\n";
  std::cerr << "[Note: Not Possible to use function templates as template arguments\n";
  std::cerr << "       Hence Not Possible to call passed function on arbitary object\n";
  std::cerr << "       without explicitly specifying type, which is ugly syntax]\n";
  std::cerr << "-------------------------\n";

  // ****************************************************************
  // TEST: SFINAE Functionality for non-generic (C++11) lambdas
  // ****************************************************************
  auto IsUntagged = [](const std::string& str_prefix) {
    return str_prefix + std::string {" NO associated tag value"};
  };

  std::cerr << "#### MAP String ####\n";

  Seven {};
  std::cerr << CTM_sz::ConditionallyExecute<Seven>::Execute(IsTagged, IsUntagged, "Given type has") << '\n';
  std::cerr << "-------------------------\n";

  Five {};
  std::cerr << CTM_sz::ConditionallyExecute<Five>::Execute(IsTagged, IsUntagged, "Given type has") << '\n';
  std::cerr << "-------------------------\n";

  std::cerr << "#### MAP Double ####\n";

  Seven {};
  std::cerr << CTM_db::ConditionallyExecute<Seven>::Execute(IsTagged, IsUntagged, "Given type has") << '\n';
  std::cerr << "-------------------------\n";

  Five {};
  std::cerr << CTM_db::ConditionallyExecute<Five>::Execute(IsTagged, IsUntagged, "Given type has") << '\n';
  std::cerr << "-------------------------\n";

  std::cerr << "-------------------------\n";
  std::cerr << "-------------------------\n";
  std::cerr << "Testing SFINAE Functionality for templated functions\n";
  std::cerr << "[Note: After wrapping operator() template in a struct it is possible to\n";
  std::cerr << "       call passed functor on arbitary parameters without explicitly\n";
  std::cerr << "       specifying type. Now we can selectively print the tag for those it\n";
  std::cerr << "       is available. Generic lambdas of C++14 will solve this too]\n";
  std::cerr << "-------------------------\n";

  std::cerr << "#### MAP String ####\n";

  // --------------------------------------------------------------------------
  // Hard-error: Intentional; to catch invalid call after SFINAE dispatch at
  //             compile time. SFINAE dispatch chooses CallableForTagged for which
  //             the following will fail.
  // Three {};
  // std::cerr << CTM_sz::ConditionallyExecute<Three>::Execute(CallableForTagged<CTM_sz> {},
  //                                                          CallableForUntagged {},
  //                                                          "Given type has") << '\n';
  // std::cerr << "-------------------------\n";
  // --------------------------------------------------------------------------

  Six {};
  std::cerr << CTM_sz::ConditionallyExecute<Six>::Execute(CallableForTagged<CTM_sz> {},
                                                 CallableForUntagged {},
                                                 "Given type has") << '\n';
  std::cerr << "-------------------------\n";

  std::cerr << CTM_sz::ConditionallyExecute<Four>::Execute(CallableForTagged<CTM_sz> {},
                                                  CallableForUntagged {},
                                                  "Given type has",
                                                  Four {}) << '\n';
  std::cerr << "-------------------------\n";

  std::cerr << CTM_sz::ConditionallyExecute<Seven>::Execute(CallableForTagged<CTM_sz> {},
                                                  CallableForUntagged {},
                                                  "Given type has",
                                                  Seven {}) << '\n';
  std::cerr << "-------------------------\n";

  std::cerr << CTM_sz::ConditionallyExecute<Zero>::Execute(CallableForTagged<CTM_sz> {},
                                                  CallableForUntagged {},
                                                  "Given type has",
                                                  Zero {}) << '\n';
  std::cerr << "-------------------------\n";


  std::cerr << "#### MAP Double ####\n";

  // --------------------------------------------------------------------------
  // Hard-error: Intentional; to catch invalid call after SFINAE dispatch at
  //             compile time. SFINAE dispatch chooses CallableForTagged for which
  //             the following will fail.
  // Three {};
  // std::cerr << CTM_db::ConditionallyExecute<Three>::Execute(CallableForTagged<CTM_db> {},
  //                                                          CallableForUntagged {},
  //                                                          "Given type has") << '\n';
  // std::cerr << "-------------------------\n";
  // --------------------------------------------------------------------------

  Six {};
  std::cerr << CTM_db::ConditionallyExecute<Six>::Execute(CallableForTagged<CTM_db> {},
                                                 CallableForUntagged {},
                                                 "Given type has") << '\n';
  std::cerr << "-------------------------\n";

  std::cerr << CTM_db::ConditionallyExecute<Four>::Execute(CallableForTagged<CTM_db> {},
                                                  CallableForUntagged {},
                                                  "Given type has",
                                                  Four {}) << '\n';
  std::cerr << "-------------------------\n";

  std::cerr << CTM_db::ConditionallyExecute<Seven>::Execute(CallableForTagged<CTM_db> {},
                                                  CallableForUntagged {},
                                                  "Given type has",
                                                  Seven {}) << '\n';
  std::cerr << "-------------------------\n";

  std::cerr << CTM_db::ConditionallyExecute<Zero>::Execute(CallableForTagged<CTM_db> {},
                                                  CallableForUntagged {},
                                                  "Given type has",
                                                  Zero {}) << '\n';
  std::cerr << "-------------------------" << std::endl;
}
