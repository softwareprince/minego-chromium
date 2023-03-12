// Copyright 2012 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_WEBBINANCE_WEBBINANCE_EXPORT_H_
#define UI_VIEWS_CONTROLS_WEBBINANCE_WEBBINANCE_EXPORT_H_

// Defines WEBBINANCE_EXPORT so that functionality implemented by the webbinance
// module can be exported to consumers.

#if defined(COMPONENT_BUILD)
#if defined(WIN32)

#if defined(WEBBINANCE_IMPLEMENTATION)
#define WEBBINANCE_EXPORT __declspec(dllexport)
#else
#define WEBBINANCE_EXPORT __declspec(dllimport)
#endif  // defined(WEBBINANCE_IMPLEMENTATION)

#else  // defined(WIN32)
#if defined(WEBBINANCE_IMPLEMENTATION)
#define WEBBINANCE_EXPORT __attribute__((visibility("default")))
#else
#define WEBBINANCE_EXPORT
#endif
#endif

#else  // defined(COMPONENT_BUILD)
#define WEBBINANCE_EXPORT
#endif

#endif  // UI_VIEWS_CONTROLS_WEBBINANCE_WEBBINANCE_EXPORT_H_
