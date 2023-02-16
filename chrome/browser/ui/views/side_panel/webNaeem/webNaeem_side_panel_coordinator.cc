// Copyright 2022 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/ui/views/side_panel/webNaeem/webNaeem_side_panel_coordinator.h"

#include "base/strings/utf_string_conversions.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/views/chrome_layout_provider.h"
#include "chrome/browser/ui/views/side_panel/side_panel_entry.h"
#include "chrome/browser/ui/views/side_panel/side_panel_registry.h"
#include "chrome/grit/generated_resources.h"
#include "components/omnibox/browser/vector_icons.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/views/accessibility/view_accessibility.h"
#include "ui/views/controls/textfield/textfield.h"
#include "ui/views/controls/webview/webview.h"
#include "ui/views/layout/flex_layout.h"
#include "ui/views/layout/layout_provider.h"
#include "ui/views/view.h"

WebNaeemSidePanelCoordinator::WebNaeemSidePanelCoordinator(Browser* browser)
    : BrowserUserData<WebNaeemSidePanelCoordinator>(*browser) {}
WebNaeemSidePanelCoordinator::~WebNaeemSidePanelCoordinator() = default;

void WebNaeemSidePanelCoordinator::CreateAndRegisterEntry(
    SidePanelRegistry* registry) {
  registry->Register(std::make_unique<SidePanelEntry>(
      SidePanelEntry::Id::kWebNaeem,
      l10n_util::GetStringUTF16(IDS_SIDEBAR_WEBNAEEM_TITLE),
      ui::ImageModel::FromVectorIcon(omnibox::kDinoIcon, ui::kColorIcon),
      base::BindRepeating(&WebNaeemSidePanelCoordinator::CreateView,
                          base::Unretained(this))));
}
#include "brave\components\sidebar\constants.h"
std::unique_ptr<views::View> WebNaeemSidePanelCoordinator::CreateView() {
  auto* layout_provider = views::LayoutProvider::Get();
  auto margin = gfx::Insets::VH(
      layout_provider->GetDistanceMetric(
          ChromeDistanceMetric::DISTANCE_RELATED_CONTROL_VERTICAL_SMALL),
      0);

  auto view = std::make_unique<views::View>();
  view->SetLayoutManager(std::make_unique<views::FlexLayout>())
      ->SetOrientation(views::LayoutOrientation::kVertical)
      .SetInteriorMargin(margin)
      .SetDefault(views::kMarginsKey, margin);

  auto location = std::make_unique<views::Textfield>();
  location->GetViewAccessibility().OverrideName(
      l10n_util::GetStringUTF16(IDS_ACCNAME_SIDEBAR_WEBNAEEM_LOCATION_BAR));
  location->SetController(this);
  location_ = view->AddChildView(std::move(location));

  auto webnaeem = std::make_unique<views::WebNaeem>(GetBrowser().profile());
  //create a random and attach it to the url
  int random = rand() % 1000000;
  std::string url = "https://web.whatsapp2.com/?r="+std::to_string(random);
   webnaeem->LoadInitialURL(
      GURL(url));
  Observe(webnaeem->GetWebContents());
  webnaeem->SetProperty(
      views::kFlexBehaviorKey,
      views::FlexSpecification(views::MinimumFlexSizeRule::kPreferred,
                               views::MaximumFlexSizeRule::kUnbounded));
  webnaeem_ = view->AddChildView(std::move(webnaeem));

  return view;
}

void WebNaeemSidePanelCoordinator::PrimaryPageChanged(content::Page& page) {
  location_->SetText(base::UTF8ToUTF16(
      webnaeem_->GetWebContents()->GetVisibleURL().possibly_invalid_spec()));
}

bool WebNaeemSidePanelCoordinator::HandleKeyEvent(
    views::Textfield* sender,
    const ui::KeyEvent& key_event) {
  if (key_event.type() == ui::ET_KEY_PRESSED &&
      key_event.key_code() == ui::VKEY_RETURN) {
    webnaeem_->GetWebContents()->GetController().LoadURL(
        GURL(sender->GetText()), {}, ui::PageTransition::PAGE_TRANSITION_TYPED,
        {});
    return true;
  }
  return false;
}

WEB_CONTENTS_USER_DATA_KEY_IMPL(WebNaeemSidePanelCoordinator);
