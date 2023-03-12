// Copyright 2022 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_SIDE_PANEL_WEBBINANCE_WEBBINANCE_SIDE_PANEL_COORDINATOR_H_
#define CHROME_BROWSER_UI_VIEWS_SIDE_PANEL_WEBBINANCE_WEBBINANCE_SIDE_PANEL_COORDINATOR_H_

#include "chrome/browser/ui/browser_user_data.h"
#include "content/public/browser/web_contents_observer.h"
#include "ui/views/controls/textfield/textfield_controller.h"

class Browser;
class SidePanelRegistry;

namespace views {
class Textfield;
class View;
class WebBinance;
}  // namespace views

class WebBinanceSidePanelCoordinator
    : public BrowserUserData<WebBinanceSidePanelCoordinator>,
      public content::WebContentsObserver,
      public views::TextfieldController {
 public:
  explicit WebBinanceSidePanelCoordinator(Browser* browser);
  WebBinanceSidePanelCoordinator(const WebBinanceSidePanelCoordinator&) = delete;
  WebBinanceSidePanelCoordinator& operator=(const WebBinanceSidePanelCoordinator&) =
      delete;
  ~WebBinanceSidePanelCoordinator() override;

  void CreateAndRegisterEntry(SidePanelRegistry* registry);

  // content::WebContentsObserver:
  void PrimaryPageChanged(content::Page& page) override;

  // views::TextfieldController:
  bool HandleKeyEvent(views::Textfield* sender,
                      const ui::KeyEvent& key_event) override;

 private:
  friend class BrowserUserData<WebBinanceSidePanelCoordinator>;

  std::unique_ptr<views::View> CreateView();

  views::Textfield* location_;
  views::WebBinance* webbinance_;

  BROWSER_USER_DATA_KEY_DECL();
};

#endif  // CHROME_BROWSER_UI_VIEWS_SIDE_PANEL_WEBBINANCE_WEBBINANCE_SIDE_PANEL_COORDINATOR_H_
