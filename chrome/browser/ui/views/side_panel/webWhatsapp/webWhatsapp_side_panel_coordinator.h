// Copyright 2022 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_SIDE_PANEL_WEBWHATSAPP_WEBWHATSAPP_SIDE_PANEL_COORDINATOR_H_
#define CHROME_BROWSER_UI_VIEWS_SIDE_PANEL_WEBWHATSAPP_WEBWHATSAPP_SIDE_PANEL_COORDINATOR_H_

#include "chrome/browser/ui/browser_user_data.h"
#include "content/public/browser/web_contents_observer.h"
#include "ui/views/controls/textfield/textfield_controller.h"

class Browser;
class SidePanelRegistry;

namespace views {
class Textfield;
class View;
class WebWhatsapp;
}  // namespace views

class WebWhatsappSidePanelCoordinator
    : public BrowserUserData<WebWhatsappSidePanelCoordinator>,
      public content::WebContentsObserver,
      public views::TextfieldController {
 public:
  explicit WebWhatsappSidePanelCoordinator(Browser* browser);
  WebWhatsappSidePanelCoordinator(const WebWhatsappSidePanelCoordinator&) = delete;
  WebWhatsappSidePanelCoordinator& operator=(const WebWhatsappSidePanelCoordinator&) =
      delete;
  ~WebWhatsappSidePanelCoordinator() override;

  void CreateAndRegisterEntry(SidePanelRegistry* registry);

  // content::WebContentsObserver:
  void PrimaryPageChanged(content::Page& page) override;

  // views::TextfieldController:
  bool HandleKeyEvent(views::Textfield* sender,
                      const ui::KeyEvent& key_event) override;

 private:
  friend class BrowserUserData<WebWhatsappSidePanelCoordinator>;

  std::unique_ptr<views::View> CreateView();

  views::Textfield* location_;
  views::WebWhatsapp* webwhatsapp_;

  BROWSER_USER_DATA_KEY_DECL();
};

#endif  // CHROME_BROWSER_UI_VIEWS_SIDE_PANEL_WEBWHATSAPP_WEBWHATSAPP_SIDE_PANEL_COORDINATOR_H_
