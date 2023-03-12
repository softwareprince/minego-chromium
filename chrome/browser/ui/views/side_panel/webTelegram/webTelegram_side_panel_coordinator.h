// Copyright 2022 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_SIDE_PANEL_WEBTELEGRAM_WEBTELEGRAM_SIDE_PANEL_COORDINATOR_H_
#define CHROME_BROWSER_UI_VIEWS_SIDE_PANEL_WEBTELEGRAM_WEBTELEGRAM_SIDE_PANEL_COORDINATOR_H_

#include "chrome/browser/ui/browser_user_data.h"
#include "content/public/browser/web_contents_observer.h"
#include "ui/views/controls/textfield/textfield_controller.h"

class Browser;
class SidePanelRegistry;

namespace views {
class Textfield;
class View;
class WebTelegram;
}  // namespace views

class WebTelegramSidePanelCoordinator
    : public BrowserUserData<WebTelegramSidePanelCoordinator>,
      public content::WebContentsObserver,
      public views::TextfieldController {
 public:
  explicit WebTelegramSidePanelCoordinator(Browser* browser);
  WebTelegramSidePanelCoordinator(const WebTelegramSidePanelCoordinator&) = delete;
  WebTelegramSidePanelCoordinator& operator=(const WebTelegramSidePanelCoordinator&) =
      delete;
  ~WebTelegramSidePanelCoordinator() override;

  void CreateAndRegisterEntry(SidePanelRegistry* registry);

  // content::WebContentsObserver:
  void PrimaryPageChanged(content::Page& page) override;

  // views::TextfieldController:
  bool HandleKeyEvent(views::Textfield* sender,
                      const ui::KeyEvent& key_event) override;

 private:
  friend class BrowserUserData<WebTelegramSidePanelCoordinator>;

  std::unique_ptr<views::View> CreateView();

  views::Textfield* location_;
  views::WebTelegram* webtelegram_;

  BROWSER_USER_DATA_KEY_DECL();
};

#endif  // CHROME_BROWSER_UI_VIEWS_SIDE_PANEL_WEBTELEGRAM_WEBTELEGRAM_SIDE_PANEL_COORDINATOR_H_
