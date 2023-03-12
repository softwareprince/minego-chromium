// Copyright 2012 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_WEBWHATSAPP_WEBWHATSAPP_EXPORT_H_
#define UI_VIEWS_CONTROLS_WEBWHATSAPP_WEBWHATSAPP_EXPORT_H_

// Defines WEBWHATSAPP_EXPORT so that functionality implemented by the webwhatsapp
// module can be exported to consumers.

#if defined(COMPONENT_BUILD)
#if defined(WIN32)

#if defined(WEBWHATSAPP_IMPLEMENTATION)
#define WEBWHATSAPP_EXPORT __declspec(dllexport)
#else
#define WEBWHATSAPP_EXPORT __declspec(dllimport)
#endif  // defined(WEBWHATSAPP_IMPLEMENTATION)

#else  // defined(WIN32)
#if defined(WEBWHATSAPP_IMPLEMENTATION)
#define WEBWHATSAPP_EXPORT __attribute__((visibility("default")))
#else
#define WEBWHATSAPP_EXPORT
#endif
#endif

#else  // defined(COMPONENT_BUILD)
#define WEBWHATSAPP_EXPORT
#endif

#endif  // UI_VIEWS_CONTROLS_WEBWHATSAPP_WEBWHATSAPP_EXPORT_H_
