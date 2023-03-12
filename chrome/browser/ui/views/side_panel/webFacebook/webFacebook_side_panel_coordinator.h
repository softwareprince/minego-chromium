// Copyright 2022 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_SIDE_PANEL_WEBFACEBOOK_WEBFACEBOOK_SIDE_PANEL_COORDINATOR_H_
#define CHROME_BROWSER_UI_VIEWS_SIDE_PANEL_WEBFACEBOOK_WEBFACEBOOK_SIDE_PANEL_COORDINATOR_H_

#include "chrome/browser/ui/browser_user_data.h"
#include "content/public/browser/web_contents_observer.h"
#include "ui/views/controls/textfield/textfield_controller.h"

class Browser;
class SidePanelRegistry;

namespace views {
class Textfield;
class View;
class WebFacebook;
}  // namespace views

class WebFacebookSidePanelCoordinator
    : public BrowserUserData<WebFacebookSidePanelCoordinator>,
      public content::WebContentsObserver,
      public views::TextfieldController {
 public:
  explicit WebFacebookSidePanelCoordinator(Browser* browser);
  WebFacebookSidePanelCoordinator(const WebFacebookSidePanelCoordinator&) = delete;
  WebFacebookSidePanelCoordinator& operator=(const WebFacebookSidePanelCoordinator&) =
      delete;
  ~WebFacebookSidePanelCoordinator() override;

  void CreateAndRegisterEntry(SidePanelRegistry* registry);

  // content::WebContentsObserver:
  void PrimaryPageChanged(content::Page& page) override;

  // views::TextfieldController:
  bool HandleKeyEvent(views::Textfield* sender,
                      const ui::KeyEvent& key_event) override;

 private:
  friend class BrowserUserData<WebFacebookSidePanelCoordinator>;

  std::unique_ptr<views::View> CreateView();

  views::Textfield* location_;
  views::WebFacebook* webfacebook_;

  BROWSER_USER_DATA_KEY_DECL();
};

#endif  // CHROME_BROWSER_UI_VIEWS_SIDE_PANEL_WEBFACEBOOK_WEBFACEBOOK_SIDE_PANEL_COORDINATOR_H_
