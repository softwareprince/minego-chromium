// Copyright 2012 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_WEBTELEGRAM_WEBTELEGRAM_EXPORT_H_
#define UI_VIEWS_CONTROLS_WEBTELEGRAM_WEBTELEGRAM_EXPORT_H_

// Defines WEBTELEGRAM_EXPORT so that functionality implemented by the webtelegram
// module can be exported to consumers.

#if defined(COMPONENT_BUILD)
#if defined(WIN32)

#if defined(WEBTELEGRAM_IMPLEMENTATION)
#define WEBTELEGRAM_EXPORT __declspec(dllexport)
#else
#define WEBTELEGRAM_EXPORT __declspec(dllimport)
#endif  // defined(WEBTELEGRAM_IMPLEMENTATION)

#else  // defined(WIN32)
#if defined(WEBTELEGRAM_IMPLEMENTATION)
#define WEBTELEGRAM_EXPORT __attribute__((visibility("default")))
#else
#define WEBTELEGRAM_EXPORT
#endif
#endif

#else  // defined(COMPONENT_BUILD)
#define WEBTELEGRAM_EXPORT
#endif

#endif  // UI_VIEWS_CONTROLS_WEBTELEGRAM_WEBTELEGRAM_EXPORT_H_
