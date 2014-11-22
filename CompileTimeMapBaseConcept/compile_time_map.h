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
 *
 *  --------------------------------------------------------------------------------------
 * | Warning: This is in no way a claim to accurately define the rules, but a set of      |
 * |          explanations that have helped me form a mental model to distinguish between |
 * |          SFINAE unfriendly hard-errors (usually instantiation errors) and SFINAE     |
 * |          friendly soft-errors. This in-turn helps build different constructs         |
 * |          accordingly - SFINAE unfriendly version I've hosted in code-project:        |
 * |          [ http://www.codeproject.com/Articles/598107/Compile-Time-Map ]             |
 * |          or a similar one done in SFINAE friendly way in this project.               |
 *  --------------------------------------------------------------------------------------
 *                               S F I N A E   R u l e :
 *                              -------------------------
 * Template will be implicitly or explicitly instantiated for working out the overload
 * resolution (either for function templates or class template specialization). In that process,
 * the instantiated template must be well formed. Once that is established then
 * <1> for function templates if access of features of that template in either function return
 *     type or function parameter is invalid
 * <2> for class specializations if access of features of that template in template arg. list
 *     is invalid
 * then SFINAE will kick in to select the best overload resolution.
 *
 * So
 * <1> Functions:
 *       template<typename T>
 *       <....> func(....) {}
 *     Instantiation of T with actual type must be completely valid then if things (access of template)
 *     in .... fail, it's SFINAE. (eg typename T::type func(....) {} etc where T might not have T::type)
 *
 * <2> Class Template Specializations:
 *       template<typename T>
 *       struct UserDefined<T, typename One<Two<typename T::type>::value>::type> {};
 *     All implicit instantiations must be valid, ie., T alone should be completely valid (imagine you
 *     did T obj {}; somewhere. This should be doable - well not exactly if default ctor is inaccessible
 *     etc., but just to explain the point), then check access: typename T::type. If that's
 *     not valid then SFINAE. Else Two<typename T::type> must be completely valid, ie
 *     Two<typename T::type> obj {}; should be doable (again not exactly - refer above). If not then
 *     hard error. Else check access: Two<....>::value. If not valid then SFINAE. Else One<.....> must be
 *     valid. Then check access: typename One<....>::type. If not then SFINAE. Else this specialization
 *     is marked to be considered and next specialization is inspected for more specialized.
 *
 * That is why choose compile-time-map and other facilities carefully to either be designed with
 * recursive body expansion (eg., using A = typename Get<T, U>::A; etc inside body) or inheritance
 * expansion (eg., struct B<.....> : B<....> {};). Latter can take part in SFINAE while former will
 * always be a hard error.
 *
 * Also note that, in class template spcializations you can use non-types but they should not depend
 * on template types. This is limitation of the language.
 *
 * template<typename, int = 1> struct A;
 * template<typename T> struct A<T, 1>;  // more specialized
 * template,typename T> struct A<T, T::value>; // compile error - non-type shouldn't depend on T.
 **************************************************************************************************/

#pragma once

#include <utility>

// ****************************************************************
//                        Data Segment
// ****************************************************************
enum class Tag : unsigned char {
  e0,  e1,  e2,  e3,  e4,  e5,  e6,  e7,  e8,  e9,
  e10, e11, e12, e13, e14, e15, e16, e17, e18, e19
};

struct Zero; struct One; struct Two; struct Three; struct Four; struct Five;

namespace detail {

  // ****************************************************************
  //                        Map Segment
  // ****************************************************************
  template<typename Data, typename NextNode> struct Node;
  struct ERROR_Entry_Not_Found_In_Map;

  template<Tag, typename> struct Pair;

  template<typename...> struct CompileTimeMap;

  template<Tag tag, typename Type, typename... Pairs>
  struct CompileTimeMap<Pair<tag, Type>, Pairs...> {
    using Map = Node<Pair<tag, Type>, typename CompileTimeMap<Pairs...>::Map>;
  };

  template<Tag tag, typename Type>
  struct CompileTimeMap<Pair<tag, Type>> {
    using Map = Node<Pair<tag, Type>, ERROR_Entry_Not_Found_In_Map>;
  };

  // ****************************************************************
  //                        Search Segment
  // ****************************************************************
  template<Tag, typename> struct GivenTagFindType;

  template<Tag GivenTag, Tag tag, typename Type, typename NextNode>
  struct GivenTagFindType<GivenTag, Node<Pair<tag, Type>, NextNode>>
      : GivenTagFindType<GivenTag, NextNode> {};

  template<Tag GivenTag, typename Type, typename NextNode>
  struct GivenTagFindType<GivenTag, Node<Pair<GivenTag, Type>, NextNode>> {
    using type = Type;
  };

  template<Tag GivenTag>
  struct GivenTagFindType<GivenTag, ERROR_Entry_Not_Found_In_Map> {};

  // ----------------------------------------------------------------

  template<typename, typename> struct GivenTypeFindTag;

  template<typename GivenType, Tag tag, typename Type, typename NextNode>
  struct GivenTypeFindTag<GivenType, Node<Pair<tag, Type>, NextNode>>
      : GivenTypeFindTag<GivenType, NextNode> {};

  template<typename GivenType, Tag tag, typename NextNode>
  struct GivenTypeFindTag<GivenType, Node<Pair<tag, GivenType>, NextNode>> {
    static const Tag value {tag};
  };

  template<typename GivenType>
  struct GivenTypeFindTag<GivenType, ERROR_Entry_Not_Found_In_Map> {};

  // ****************************************************************
  //                        Populate Segment
  // ****************************************************************
  using RootNode = CompileTimeMap<
  Pair<Tag::e0, Zero>, Pair<Tag::e1, One>, Pair<Tag::e2, Two>, Pair<Tag::e3, Three>,
  Pair<Tag::e4, Four>, Pair<Tag::e5, Five>
  >::Map;

  // ****************************************************************
  //                   SFINAE-Branching Helpers Segment
  // ****************************************************************
  template<Tag... tags> struct TypesToVoid { using type = void; };
  template<Tag... tags> using void_t = typename TypesToVoid<tags...>::type;

}  // namespace detail

// ****************************************************************
//                        Convenience Segment
// ****************************************************************
template<Tag tag>
using GivenTagFindType_t = typename detail::GivenTagFindType<tag, detail::RootNode>::type;

template<typename Type>
struct GivenTypeFindTag_v : detail::GivenTypeFindTag<Type, detail::RootNode> {};

// ****************************************************************
//                      SFINAE-Branching Segment
// ****************************************************************
template<typename Type, typename = void>
struct ConditionallyExecute {
  template<typename CallIfFound, typename CallIfNotFound, typename... Types>
  static auto Execute(CallIfFound, CallIfNotFound F, Types&&... args) -> decltype(F(args...)) {
    return F(std::forward<Types>(args)...);
  }
};

template<typename Type>
struct ConditionallyExecute<Type, detail::void_t<GivenTypeFindTag_v<Type>::value>> {
  template<typename CallIfFound, typename CallIfNotFound, typename... Types>
  static auto Execute(CallIfFound F, CallIfNotFound, Types&&... args) -> decltype(F(args...)) {
    return F(std::forward<Types>(args)...);
  }
};
