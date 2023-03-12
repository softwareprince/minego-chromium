// Copyright 2012 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_WEBFACEBOOKMESSENGER_WEBFACEBOOKMESSENGER_EXPORT_H_
#define UI_VIEWS_CONTROLS_WEBFACEBOOKMESSENGER_WEBFACEBOOKMESSENGER_EXPORT_H_

// Defines WEBFACEBOOKMESSENGER_EXPORT so that functionality implemented by the webfacebookmessenger
// module can be exported to consumers.

#if defined(COMPONENT_BUILD)
#if defined(WIN32)

#if defined(WEBFACEBOOKMESSENGER_IMPLEMENTATION)
#define WEBFACEBOOKMESSENGER_EXPORT __declspec(dllexport)
#else
#define WEBFACEBOOKMESSENGER_EXPORT __declspec(dllimport)
#endif  // defined(WEBFACEBOOKMESSENGER_IMPLEMENTATION)

#else  // defined(WIN32)
#if defined(WEBFACEBOOKMESSENGER_IMPLEMENTATION)
#define WEBFACEBOOKMESSENGER_EXPORT __attribute__((visibility("default")))
#else
#define WEBFACEBOOKMESSENGER_EXPORT
#endif
#endif

#else  // defined(COMPONENT_BUILD)
#define WEBFACEBOOKMESSENGER_EXPORT
#endif

#endif  // UI_VIEWS_CONTROLS_WEBFACEBOOKMESSENGER_WEBFACEBOOKMESSENGER_EXPORT_H_
