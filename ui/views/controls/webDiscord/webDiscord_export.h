// Copyright 2012 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_WEBDISCORD_WEBDISCORD_EXPORT_H_
#define UI_VIEWS_CONTROLS_WEBDISCORD_WEBDISCORD_EXPORT_H_

// Defines WEBDISCORD_EXPORT so that functionality implemented by the webdiscord
// module can be exported to consumers.

#if defined(COMPONENT_BUILD)
#if defined(WIN32)

#if defined(WEBDISCORD_IMPLEMENTATION)
#define WEBDISCORD_EXPORT __declspec(dllexport)
#else
#define WEBDISCORD_EXPORT __declspec(dllimport)
#endif  // defined(WEBDISCORD_IMPLEMENTATION)

#else  // defined(WIN32)
#if defined(WEBDISCORD_IMPLEMENTATION)
#define WEBDISCORD_EXPORT __attribute__((visibility("default")))
#else
#define WEBDISCORD_EXPORT
#endif
#endif

#else  // defined(COMPONENT_BUILD)
#define WEBDISCORD_EXPORT
#endif

#endif  // UI_VIEWS_CONTROLS_WEBDISCORD_WEBDISCORD_EXPORT_H_
