// Copyright 2012 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_WEBFACEBOOK_WEBFACEBOOK_EXPORT_H_
#define UI_VIEWS_CONTROLS_WEBFACEBOOK_WEBFACEBOOK_EXPORT_H_

// Defines WEBFACEBOOK_EXPORT so that functionality implemented by the webfacebook
// module can be exported to consumers.

#if defined(COMPONENT_BUILD)
#if defined(WIN32)

#if defined(WEBFACEBOOK_IMPLEMENTATION)
#define WEBFACEBOOK_EXPORT __declspec(dllexport)
#else
#define WEBFACEBOOK_EXPORT __declspec(dllimport)
#endif  // defined(WEBFACEBOOK_IMPLEMENTATION)

#else  // defined(WIN32)
#if defined(WEBFACEBOOK_IMPLEMENTATION)
#define WEBFACEBOOK_EXPORT __attribute__((visibility("default")))
#else
#define WEBFACEBOOK_EXPORT
#endif
#endif

#else  // defined(COMPONENT_BUILD)
#define WEBFACEBOOK_EXPORT
#endif

#endif  // UI_VIEWS_CONTROLS_WEBFACEBOOK_WEBFACEBOOK_EXPORT_H_
