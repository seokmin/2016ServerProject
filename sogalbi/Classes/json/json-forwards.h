/// Json-cpp amalgated forward header (http://jsoncpp.sourceforge.net/).
/// It is intended to be used with #include "json/json-forwards.h"
/// This header provides forward declaration for all JsonCpp types.

// //////////////////////////////////////////////////////////////////////
// Beginning of content of file: LICENSE
// //////////////////////////////////////////////////////////////////////

/*
The JsonCpp library's source code, including accompanying documentation, 
tests and demonstration applications, are licensed under the following
conditions...

The author (Baptiste Lepilleur) explicitly disclaims copyright in all 
jurisdictions which recognize such a disclaimer. In such jurisdictions, 
this software is released into the Public Domain.

In jurisdictions which do not recognize Public Domain property (e.g. Germany as of
2010), this software is Copyright (c) 2007-2010 by Baptiste Lepilleur, and is
released under the terms of the MIT License (see below).

In jurisdictions which recognize Public Domain property, the user of this 
software may choose to accept it either as 1) Public Domain, 2) under the 
conditions of the MIT License (see below), or 3) under the terms of dual 
Public Domain/MIT License conditions described here, as they choose.

The MIT License is about as close to Public Domain as a license can get, and is
described in clear, concise terms at:

   http://en.wikipedia.org/wiki/MITLicense
   
The full text of the MIT License follows:

========================================================================
Copyright (c) 2007-2010 Baptiste Lepilleur

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
========================================================================
(END LICENSE TEXT)

The MIT license is compatible with both the GPL and commercial
software, affording one all of the rights of Public Domain with the
minor nuisance of being required to keep the above copyright notice
and license text in the source code. Note also that by accepting the
Public Domain "license" you can re-license your copy using whatever
license you like.

*/

// //////////////////////////////////////////////////////////////////////
// End of content of file: LICENSE
// //////////////////////////////////////////////////////////////////////





#ifndef JSONFORWARDAMALGATEDHINCLUDED
# define JSONFORWARDAMALGATEDHINCLUDED
/// If defined, indicates that the source file is amalgated
/// to prevent private header inclusion.
#define JSONISAMALGAMATION

// //////////////////////////////////////////////////////////////////////
// Beginning of content of file: include/json/config.h
// //////////////////////////////////////////////////////////////////////

// Copyright 2007-2010 Baptiste Lepilleur
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at http://jsoncpp.sourceforge.net/LICENSE

#ifndef JSONCONFIGHINCLUDED
#define JSONCONFIGHINCLUDED
#include <stddef.h>
#include <string> //typdef String

/// If defined, indicates that json library is embedded in CppTL library.
//# define JSONINCPPTL 1

/// If defined, indicates that json may leverage CppTL library
//#  define JSONUSECPPTL 1
/// If defined, indicates that cpptl vector based map should be used instead of
/// std::map
/// as Value container.
//#  define JSONUSECPPTLSMALLMAP 1

// If non-zero, the library uses exceptions to report bad input instead of C
// assertion macros. The default is to use exceptions.
#ifndef JSONUSEEXCEPTION
#define JSONUSEEXCEPTION 1
#endif

/// If defined, indicates that the source file is amalgated
/// to prevent private header inclusion.
/// Remarks: it is automatically defined in the generated amalgated header.
// #define JSONISAMALGAMATION

#ifdef JSONINCPPTL
#include <cpptl/config.h>
#ifndef JSONUSECPPTL
#define JSONUSECPPTL 1
#endif
#endif

#ifdef JSONINCPPTL
#define JSONAPI CPPTLAPI
#elif defined(JSONDLLBUILD)
#if defined(MSCVER) || defined(MINGW32)
#define JSONAPI declspec(dllexport)
#define JSONCPPDISABLEDLLINTERFACEWARNING
#endif // if defined(MSCVER)
#elif defined(JSONDLL)
#if defined(MSCVER) || defined(MINGW32)
#define JSONAPI declspec(dllimport)
#define JSONCPPDISABLEDLLINTERFACEWARNING
#endif // if defined(MSCVER)
#endif // ifdef JSONINCPPTL
#if !defined(JSONAPI)
#define JSONAPI
#endif

// If JSONNOINT64 is defined, then Json only support C++ "int" type for
// integer
// Storages, and 64 bits integer support is disabled.
// #define JSONNOINT64 1

#if defined(MSCVER) // MSVC
#  if MSCVER <= 1200 // MSVC 6
    // Microsoft Visual Studio 6 only support conversion from int64 to double
    // (no conversion from unsigned int64).
#    define JSONUSEINT64DOUBLECONVERSION 1
    // Disable warning 4786 for VS6 caused by STL (identifier was truncated to '255'
    // characters in the debug information)
    // All projects I've ever seen with VS6 were using this globally (not bothering
    // with pragma push/pop).
#    pragma warning(disable : 4786)
#  endif // MSVC 6

#  if MSCVER >= 1500 // MSVC 2008
    /// Indicates that the following function is deprecated.
#    define JSONCPPDEPRECATED(message) declspec(deprecated(message))
#  endif

#endif // defined(MSCVER)

// In c++11 the override keyword allows you to explicity define that a function
// is intended to override the base-class version.  This makes the code more
// managable and fixes a set of common hard-to-find bugs.
#if cplusplus >= 201103L
# define JSONCPPOVERRIDE override
#else
# define JSONCPPOVERRIDE
#endif

#ifndef JSONHASRVALUEREFERENCES

#if defined(MSCVER) && MSCVER >= 1600 // MSVC >= 2010
#define JSONHASRVALUEREFERENCES 1
#endif // MSVC >= 2010

#ifdef clang
#if hasfeature(cxxrvaluereferences)
#define JSONHASRVALUEREFERENCES 1
#endif  // hasfeature

#elif defined GNUC // not clang (gcc comes later since clang emulates gcc)
#if defined(GXXEXPERIMENTALCXX0X) || (cplusplus >= 201103L)
#define JSONHASRVALUEREFERENCES 1
#endif  // GXXEXPERIMENTAL

#endif // clang || GNUC

#endif // not defined JSONHASRVALUEREFERENCES

#ifndef JSONHASRVALUEREFERENCES
#define JSONHASRVALUEREFERENCES 0
#endif

#ifdef clang
#elif defined GNUC // not clang (gcc comes later since clang emulates gcc)
#  if (GNUC > 4 || (GNUC == 4 && GNUCMINOR >= 5))
#    define JSONCPPDEPRECATED(message)  attribute ((deprecated(message)))
#  elif (GNUC > 3 || (GNUC == 3 && GNUCMINOR >= 1))
#    define JSONCPPDEPRECATED(message)  attribute((deprecated))
#  endif  // GNUC version
#endif // clang || GNUC

#if !defined(JSONCPPDEPRECATED)
#define JSONCPPDEPRECATED(message)
#endif // if !defined(JSONCPPDEPRECATED)

#if GNUC >= 6
#  define JSONUSEINT64DOUBLECONVERSION 1
#endif

#if !defined(JSONISAMALGAMATION)

# include "version.h"

# if JSONCPPUSINGSECUREMEMORY
#  include "allocator.h" //typedef Allocator
# endif

#endif // if !defined(JSONISAMALGAMATION)

namespace Json {
typedef int Int;
typedef unsigned int UInt;
#if defined(JSONNOINT64)
typedef int LargestInt;
typedef unsigned int LargestUInt;
#undef JSONHASINT64
#else                 // if defined(JSONNOINT64)
// For Microsoft Visual use specific types as long long is not supported
#if defined(MSCVER) // Microsoft Visual Studio
typedef int64 Int64;
typedef unsigned int64 UInt64;
#else                 // if defined(MSCVER) // Other platforms, use long long
typedef long long int Int64;
typedef unsigned long long int UInt64;
#endif // if defined(MSCVER)
typedef Int64 LargestInt;
typedef UInt64 LargestUInt;
#define JSONHASINT64
#endif // if defined(JSONNOINT64)
#if JSONCPPUSINGSECUREMEMORY
#define JSONCPPSTRING        std::basicstring<char, std::chartraits<char>, Json::SecureAllocator<char> >
#define JSONCPPOSTRINGSTREAM std::basicostringstream<char, std::chartraits<char>, Json::SecureAllocator<char> >
#define JSONCPPOSTREAM       std::basicostream<char, std::chartraits<char>>
#define JSONCPPISTRINGSTREAM std::basicistringstream<char, std::chartraits<char>, Json::SecureAllocator<char> >
#define JSONCPPISTREAM       std::istream
#else
#define JSONCPPSTRING        std::string
#define JSONCPPOSTRINGSTREAM std::ostringstream
#define JSONCPPOSTREAM       std::ostream
#define JSONCPPISTRINGSTREAM std::istringstream
#define JSONCPPISTREAM       std::istream
#endif // if JSONCPPUSINGSECUREMEMORY
} // end namespace Json

#endif // JSONCONFIGHINCLUDED

// //////////////////////////////////////////////////////////////////////
// End of content of file: include/json/config.h
// //////////////////////////////////////////////////////////////////////






// //////////////////////////////////////////////////////////////////////
// Beginning of content of file: include/json/forwards.h
// //////////////////////////////////////////////////////////////////////

// Copyright 2007-2010 Baptiste Lepilleur
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at http://jsoncpp.sourceforge.net/LICENSE

#ifndef JSONFORWARDSHINCLUDED
#define JSONFORWARDSHINCLUDED

#if !defined(JSONISAMALGAMATION)
#include "config.h"
#endif // if !defined(JSONISAMALGAMATION)

namespace Json {

// writer.h
class FastWriter;
class StyledWriter;

// reader.h
class Reader;

// features.h
class Features;

// value.h
typedef unsigned int ArrayIndex;
class StaticString;
class Path;
class PathArgument;
class Value;
class ValueIteratorBase;
class ValueIterator;
class ValueConstIterator;

} // namespace Json

#endif // JSONFORWARDSHINCLUDED

// //////////////////////////////////////////////////////////////////////
// End of content of file: include/json/forwards.h
// //////////////////////////////////////////////////////////////////////





#endif //ifndef JSONFORWARDAMALGATEDHINCLUDED
