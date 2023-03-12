// Copyright 2012 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_WEBCOINMARKETCAP_WEBCOINMARKETCAP_EXPORT_H_
#define UI_VIEWS_CONTROLS_WEBCOINMARKETCAP_WEBCOINMARKETCAP_EXPORT_H_

// Defines WEBCOINMARKETCAP_EXPORT so that functionality implemented by the webcoinmarketcap
// module can be exported to consumers.

#if defined(COMPONENT_BUILD)
#if defined(WIN32)

#if defined(WEBCOINMARKETCAP_IMPLEMENTATION)
#define WEBCOINMARKETCAP_EXPORT __declspec(dllexport)
#else
#define WEBCOINMARKETCAP_EXPORT __declspec(dllimport)
#endif  // defined(WEBCOINMARKETCAP_IMPLEMENTATION)

#else  // defined(WIN32)
#if defined(WEBCOINMARKETCAP_IMPLEMENTATION)
#define WEBCOINMARKETCAP_EXPORT __attribute__((visibility("default")))
#else
#define WEBCOINMARKETCAP_EXPORT
#endif
#endif

#else  // defined(COMPONENT_BUILD)
#define WEBCOINMARKETCAP_EXPORT
#endif

#endif  // UI_VIEWS_CONTROLS_WEBCOINMARKETCAP_WEBCOINMARKETCAP_EXPORT_H_
