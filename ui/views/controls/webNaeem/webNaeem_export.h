// Copyright 2012 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_WEBNAEEM_WEBNAEEM_EXPORT_H_
#define UI_VIEWS_CONTROLS_WEBNAEEM_WEBNAEEM_EXPORT_H_

// Defines WEBNAEEM_EXPORT so that functionality implemented by the webnaeem
// module can be exported to consumers.

#if defined(COMPONENT_BUILD)
#if defined(WIN32)

#if defined(WEBNAEEM_IMPLEMENTATION)
#define WEBNAEEM_EXPORT __declspec(dllexport)
#else
#define WEBNAEEM_EXPORT __declspec(dllimport)
#endif  // defined(WEBNAEEM_IMPLEMENTATION)

#else  // defined(WIN32)
#if defined(WEBNAEEM_IMPLEMENTATION)
#define WEBNAEEM_EXPORT __attribute__((visibility("default")))
#else
#define WEBNAEEM_EXPORT
#endif
#endif

#else  // defined(COMPONENT_BUILD)
#define WEBNAEEM_EXPORT
#endif

#endif  // UI_VIEWS_CONTROLS_WEBNAEEM_WEBNAEEM_EXPORT_H_
