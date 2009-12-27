// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// FFmpeg is written in C99 and requires <stdint.h> and <inttypes.h>.
// Since MSVC doesn't include these headers, we have to write our own version
// to provide a compatibility layer between MSVC and the FFmpeg headers.

#ifndef THIRD_PARTY_FFMPEG_INCLUDE_WIN_STDINT_H_
#define THIRD_PARTY_FFMPEG_INCLUDE_WIN_STDINT_H_

#if !defined(_MSC_VER)
#error This file should only be included when compiling with MSVC.
#endif

// Define C99 equivalent types.
typedef signed char           int8_t;
typedef signed short          int16_t;
typedef signed int            int32_t;
typedef signed long long      int64_t;
typedef unsigned char         uint8_t;
typedef unsigned short        uint16_t;
typedef unsigned int          uint32_t;
typedef unsigned long long    uint64_t;

#undef int_fast8_t
#undef uint_fast8_t
#undef int_fast16_t
#undef uint_fast16_t
#undef int_fast32_t
#undef uint_fast32_t
#undef int_fast64_t
#undef uint_fast64_t
#define int_fast8_t long int
#define uint_fast8_t unsigned int_fast8_t
#define int_fast16_t long int
#define uint_fast16_t unsigned int_fast16_t
#define int_fast32_t long int
#define uint_fast32_t unsigned int_fast32_t
#define int_fast64_t int64_t
#define uint_fast64_t uint64_t


#endif  // THIRD_PARTY_FFMPEG_INCLUDE_WIN_STDINT_H_
