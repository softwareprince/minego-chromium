// Copyright 2014 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/controls/webTelegram/webTelegram.h"

#include <stdint.h>

#include <memory>
#include <utility>

#include "base/bind.h"
#include "base/command_line.h"
#include "base/memory/ptr_util.h"
#include "base/memory/raw_ptr.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/common/content_client.h"
#include "content/public/common/content_features.h"
#include "content/public/common/content_switches.h"
#include "content/public/test/browser_task_environment.h"
#include "content/public/test/mock_render_process_host.h"
#include "content/public/test/test_browser_context.h"
#include "content/public/test/test_renderer_host.h"
#include "content/public/test/web_contents_tester.h"
#include "content/test/test_content_browser_client.h"
#include "ui/events/event.h"
#include "ui/events/event_utils.h"
#include "ui/views/controls/native/native_view_host.h"
#include "ui/views/test/views_test_utils.h"
#include "ui/views/test/widget_test.h"

#if defined(USE_AURA)
#include "ui/aura/window.h"
#endif

namespace views {

namespace {

// Provides functionality to observe events on a WebContents like
// OnVisibilityChanged/WebContentsDestroyed.
class WebTelegramTestWebContentsObserver : public content::WebContentsObserver {
 public:
  explicit WebTelegramTestWebContentsObserver(content::WebContents* web_contents)
      : web_contents_(web_contents) {
    content::WebContentsObserver::Observe(web_contents);
  }

  WebTelegramTestWebContentsObserver(const WebTelegramTestWebContentsObserver&) =
      delete;
  WebTelegramTestWebContentsObserver& operator=(
      const WebTelegramTestWebContentsObserver&) = delete;

  ~WebTelegramTestWebContentsObserver() override {
    if (web_contents_)
      content::WebContentsObserver::Observe(nullptr);
  }

  void WebContentsDestroyed() override {
    DCHECK(web_contents_);
    content::WebContentsObserver::Observe(nullptr);
    web_contents_ = nullptr;
  }

  void OnVisibilityChanged(content::Visibility visibility) override {
    switch (visibility) {
      case content::Visibility::VISIBLE: {
#if defined(USE_AURA)
        valid_root_while_shown_ =
            web_contents()->GetNativeView()->GetRootWindow() != nullptr;
#endif
        was_shown_ = true;
        ++shown_count_;
        break;
      }
      case content::Visibility::HIDDEN: {
        was_shown_ = false;
        ++hidden_count_;
        break;
      }
      default: {
        ADD_FAILURE() << "Unexpected call to OnVisibilityChanged.";
        break;
      }
    }
  }

  bool was_shown() const { return was_shown_; }

  int shown_count() const { return shown_count_; }

  int hidden_count() const { return hidden_count_; }

  bool valid_root_while_shown() const { return valid_root_while_shown_; }

 private:
  raw_ptr<content::WebContents> web_contents_;
  bool was_shown_ = false;
  int32_t shown_count_ = 0;
  int32_t hidden_count_ = 0;
  // Set to true if the view containing the webcontents has a valid root window.
  bool valid_root_while_shown_ = true;
};

// Fakes the fullscreen browser state reported to WebContents and WebTelegram.
class WebTelegramTestWebContentsDelegate : public content::WebContentsDelegate {
 public:
  WebTelegramTestWebContentsDelegate() = default;

  WebTelegramTestWebContentsDelegate(const WebTelegramTestWebContentsDelegate&) =
      delete;
  WebTelegramTestWebContentsDelegate& operator=(
      const WebTelegramTestWebContentsDelegate&) = delete;

  ~WebTelegramTestWebContentsDelegate() override = default;

  void set_is_fullscreened(bool fs) { is_fullscreened_ = fs; }

  // content::WebContentsDelegate overrides.
  bool IsFullscreenForTabOrPending(
      const content::WebContents* ignored) override {
    return is_fullscreened_;
  }

 private:
  bool is_fullscreened_ = false;
};

}  // namespace

// Provides functionality to test a WebTelegram.
class WebTelegramUnitTest : public views::test::WidgetTest {
 public:
  WebTelegramUnitTest()
      : views::test::WidgetTest(std::unique_ptr<base::test::TaskEnvironment>(
            std::make_unique<content::BrowserTaskEnvironment>())) {}

  WebTelegramUnitTest(const WebTelegramUnitTest&) = delete;
  WebTelegramUnitTest& operator=(const WebTelegramUnitTest&) = delete;

  ~WebTelegramUnitTest() override = default;

  std::unique_ptr<content::WebContents> CreateWebContentsForWebTelegram(
      content::BrowserContext* browser_context) {
    return content::WebContentsTester::CreateTestWebContents(browser_context,
                                                             nullptr);
  }

  void SetUp() override {
    rvh_enabler_ = std::make_unique<content::RenderViewHostTestEnabler>();

    views::WebTelegram::WebContentsCreator creator = base::BindRepeating(
        &WebTelegramUnitTest::CreateWebContentsForWebTelegram, base::Unretained(this));
    scoped_web_contents_creator_ =
        std::make_unique<views::WebTelegram::ScopedWebContentsCreatorForTesting>(
            creator);
    browser_context_ = std::make_unique<content::TestBrowserContext>();
    WidgetTest::SetUp();
    // Set the test content browser client to avoid pulling in needless
    // dependencies from content.
    SetBrowserClientForTesting(&test_browser_client_);

    base::CommandLine::ForCurrentProcess()->AppendSwitch(
        switches::kDisableBackgroundingOccludedWindowsForTesting);

    // Create a top level widget and add a child, and give it a WebTelegram as a
    // child.
    top_level_widget_ = CreateTopLevelFramelessPlatformWidget();
    top_level_widget_->SetBounds(gfx::Rect(0, 10, 100, 100));
    View* const contents_view =
        top_level_widget_->SetContentsView(std::make_unique<View>());
    auto web_view = std::make_unique<WebTelegram>(browser_context_.get());
    web_view->SetBoundsRect(gfx::Rect(contents_view->size()));
    web_view_ = contents_view->AddChildView(std::move(web_view));
    top_level_widget_->Show();
    ASSERT_EQ(gfx::Rect(0, 0, 100, 100), web_view_->bounds());
  }

  void TearDown() override {
    scoped_web_contents_creator_.reset();
    top_level_widget_->Close();  // Deletes all children and itself.
    RunPendingMessages();

    browser_context_.reset(nullptr);
    // Flush the message loop to execute pending relase tasks as this would
    // upset ASAN and Valgrind.
    RunPendingMessages();
    WidgetTest::TearDown();
  }

 protected:
  Widget* top_level_widget() const { return top_level_widget_; }
  WebTelegram* web_view() const { return web_view_; }
  NativeViewHost* holder() const { return web_view_->holder_; }

  std::unique_ptr<content::WebContents> CreateWebContents() const {
    return content::WebContents::Create(
        content::WebContents::CreateParams(browser_context_.get()));
  }

  std::unique_ptr<content::WebContents> CreateTestWebContents() const {
    return content::WebContentsTester::CreateTestWebContents(
        browser_context_.get(), /*instance=*/nullptr);
  }

  void SetAXMode(ui::AXMode mode) { web_view()->OnAXModeAdded(mode); }

 private:
  std::unique_ptr<content::RenderViewHostTestEnabler> rvh_enabler_;
  std::unique_ptr<content::TestBrowserContext> browser_context_;
  content::TestContentBrowserClient test_browser_client_;
  std::unique_ptr<views::WebTelegram::ScopedWebContentsCreatorForTesting>
      scoped_web_contents_creator_;

  raw_ptr<Widget> top_level_widget_ = nullptr;
  raw_ptr<WebTelegram> web_view_ = nullptr;
};

// Tests that attaching and detaching a WebContents to a WebTelegram makes the
// WebContents visible and hidden respectively.
TEST_F(WebTelegramUnitTest, TestWebTelegramAttachDetachWebContents) {
  // Case 1: Create a new WebContents and set it in the webtelegram via
  // SetWebContents. This should make the WebContents visible.
  const std::unique_ptr<content::WebContents> web_contents1(
      CreateWebContents());
  WebTelegramTestWebContentsObserver observer1(web_contents1.get());
  EXPECT_FALSE(observer1.was_shown());

  web_view()->SetWebContents(web_contents1.get());
  // Layout is normally async, ensure it runs now so visibility is updated.
  views::test::RunScheduledLayout(web_view());
  EXPECT_TRUE(observer1.was_shown());
#if defined(USE_AURA)
  EXPECT_TRUE(web_contents1->GetNativeView()->IsVisible());
#endif
  EXPECT_EQ(observer1.shown_count(), 1);
  EXPECT_EQ(observer1.hidden_count(), 0);
  EXPECT_TRUE(observer1.valid_root_while_shown());

  // Case 2: Create another WebContents and replace the current WebContents
  // via SetWebContents(). This should hide the current WebContents and show
  // the new one.
  const std::unique_ptr<content::WebContents> web_contents2(
      CreateWebContents());
  WebTelegramTestWebContentsObserver observer2(web_contents2.get());
  EXPECT_FALSE(observer2.was_shown());

  // Setting the new WebContents should hide the existing one.
  web_view()->SetWebContents(web_contents2.get());
  // Layout is normally async, ensure it runs now so visibility is updated.
  views::test::RunScheduledLayout(web_view());
  EXPECT_FALSE(observer1.was_shown());
  EXPECT_TRUE(observer2.was_shown());
  EXPECT_TRUE(observer2.valid_root_while_shown());

  // WebContents1 should not get stray show calls when WebContents2 is set.
  EXPECT_EQ(observer1.shown_count(), 1);
  EXPECT_EQ(observer1.hidden_count(), 1);
  EXPECT_EQ(observer2.shown_count(), 1);
  EXPECT_EQ(observer2.hidden_count(), 0);

  // Case 3: Test that attaching to a hidden webtelegram does not show the web
  // contents.
  web_view()->SetVisible(false);
  EXPECT_EQ(1, observer2.hidden_count());  // Now hidden.

  EXPECT_EQ(1, observer1.shown_count());
  web_view()->SetWebContents(web_contents1.get());
  // Layout is normally async, ensure it runs now so visibility is updated.
  views::test::RunScheduledLayout(web_view());
  EXPECT_EQ(1, observer1.shown_count());

  // Nothing else should change.
  EXPECT_EQ(1, observer1.hidden_count());
  EXPECT_EQ(1, observer2.shown_count());
  EXPECT_EQ(1, observer2.hidden_count());

#if defined(USE_AURA)
  // Case 4: Test that making the webtelegram visible when a window has an invisible
  // parent does not make the web contents visible.
  top_level_widget()->Hide();
  web_view()->SetVisible(true);
  EXPECT_EQ(1, observer1.shown_count());
  top_level_widget()->Show();
  EXPECT_EQ(2, observer1.shown_count());
  top_level_widget()->Hide();
  EXPECT_EQ(2, observer1.hidden_count());
#else
  // On Mac, changes to window visibility do not trigger calls to WebContents::
  // WasShown() or WasHidden(), since the OS does not provide good signals for
  // window visibility. However, we can still test that moving a visible WebTelegram
  // whose WebContents is not currently showing to a new, visible window will
  // show the WebContents. Simulate the "hide window with visible WebTelegram" step
  // simply by detaching the WebContents.
  web_view()->SetVisible(true);
  EXPECT_EQ(2, observer1.shown_count());
  web_view()->holder()->Detach();
  EXPECT_EQ(2, observer1.hidden_count());
#endif
  // Case 5: Test that moving from a hidden parent to a visible parent makes the
  // web contents visible.
  Widget* parent2 = CreateTopLevelFramelessPlatformWidget();
  parent2->SetBounds(gfx::Rect(0, 10, 100, 100));
  parent2->Show();
  EXPECT_EQ(2, observer1.shown_count());
  // Note: that reparenting the windows directly, after the windows have been
  // created, e.g., Widget::ReparentNativeView(widget, parent2), is not a
  // supported use case. Instead, move the WebTelegram over.
  parent2->SetContentsView(web_view()->parent()->RemoveChildViewT(web_view()));
  EXPECT_EQ(3, observer1.shown_count());
  parent2->Close();
}

// Verifies that there is no crash in WebTelegram destructor
// if WebTelegram is already removed from Widget.
TEST_F(WebTelegramUnitTest, DetachedWebTelegramDestructor) {
  // Init WebTelegram with attached NativeView.
  const std::unique_ptr<content::WebContents> web_contents =
      CreateWebContents();
  std::unique_ptr<WebTelegram> webtelegram(
      new WebTelegram(web_contents->GetBrowserContext()));
  View* contents_view = top_level_widget()->GetContentsView();
  contents_view->AddChildView(webtelegram.get());
  webtelegram->SetWebContents(web_contents.get());

  // Remove WebTelegram from views hierarchy. NativeView should be detached
  // from Widget.
  contents_view->RemoveChildView(webtelegram.get());
  // Destroy WebTelegram. NativeView should be detached secondary.
  // There should be no crash.
  webtelegram.reset();
}

// Test that the specified crashed overlay view is shown when a WebContents
// is in a crashed state.
TEST_F(WebTelegramUnitTest, CrashedOverlayView) {
  const std::unique_ptr<content::WebContents> web_contents =
      CreateTestWebContents();
  content::WebContentsTester* tester =
      content::WebContentsTester::For(web_contents.get());

  std::unique_ptr<WebTelegram> web_view(
      new WebTelegram(web_contents->GetBrowserContext()));
  View* contents_view = top_level_widget()->GetContentsView();
  contents_view->AddChildView(web_view.get());
  web_view->SetWebContents(web_contents.get());

  View* crashed_overlay_view = new View();
  web_view->SetCrashedOverlayView(crashed_overlay_view);
  EXPECT_FALSE(crashed_overlay_view->IsDrawn());

  // Normally when a renderer crashes, the WebTelegram will learn about it
  // automatically via WebContentsObserver. Since this is a test
  // WebContents, simulate that by calling SetIsCrashed and then
  // explicitly calling RenderFrameDeleted on the WebTelegram to trigger it
  // to swap in the crashed overlay view.
  tester->SetIsCrashed(base::TERMINATION_STATUS_PROCESS_CRASHED, -1);
  EXPECT_TRUE(web_contents->IsCrashed());
  static_cast<content::WebContentsObserver*>(web_view.get())
      ->RenderFrameDeleted(web_contents->GetPrimaryMainFrame());
  EXPECT_TRUE(crashed_overlay_view->IsDrawn());
}

// Test that a crashed overlay view isn't deleted if it's owned by client.
TEST_F(WebTelegramUnitTest, CrashedOverlayViewOwnedbyClient) {
  const std::unique_ptr<content::WebContents> web_contents =
      CreateTestWebContents();
  content::WebContentsTester* tester =
      content::WebContentsTester::For(web_contents.get());
  std::unique_ptr<WebTelegram> web_view(
      new WebTelegram(web_contents->GetBrowserContext()));
  View* contents_view = top_level_widget()->GetContentsView();
  contents_view->AddChildView(web_view.get());
  web_view->SetWebContents(web_contents.get());

  View* crashed_overlay_view = new View();
  crashed_overlay_view->set_owned_by_client();
  web_view->SetCrashedOverlayView(crashed_overlay_view);
  EXPECT_FALSE(crashed_overlay_view->IsDrawn());

  // Simulate a renderer crash (see above).
  tester->SetIsCrashed(base::TERMINATION_STATUS_PROCESS_CRASHED, -1);
  EXPECT_TRUE(web_contents->IsCrashed());
  static_cast<content::WebContentsObserver*>(web_view.get())
      ->RenderFrameDeleted(web_contents->GetPrimaryMainFrame());
  EXPECT_TRUE(crashed_overlay_view->IsDrawn());

  web_view->SetCrashedOverlayView(nullptr);
  web_view.reset();

  // This shouldn't crash, we still own this.
  delete crashed_overlay_view;
}

// Tests to make sure we can default construct the WebTelegram class and set the
// BrowserContext after construction.
TEST_F(WebTelegramUnitTest, DefaultConstructability) {
  auto browser_context = std::make_unique<content::TestBrowserContext>();
  auto web_view = std::make_unique<WebTelegram>();

  // Test to make sure the WebTelegram returns a nullptr in the absence of an
  // explicitly supplied WebContents and BrowserContext.
  EXPECT_EQ(nullptr, web_view->GetWebContents());

  web_view->SetBrowserContext(browser_context.get());

  // WebTelegram should be able to create a WebContents object from the previously
  // set |browser_context|.
  auto* web_contents = web_view->GetWebContents();
  EXPECT_NE(nullptr, web_contents);
  EXPECT_EQ(browser_context.get(), web_contents->GetBrowserContext());
}

// Tests that when a web view is reparented to a different widget hierarchy its
// holder's parent NativeViewAccessible matches that of its parent view's
// NativeViewAccessible.
TEST_F(WebTelegramUnitTest, ReparentingUpdatesParentAccessible) {
  const std::unique_ptr<content::WebContents> web_contents =
      CreateWebContents();
  auto web_view = std::make_unique<WebTelegram>(web_contents->GetBrowserContext());
  web_view->SetWebContents(web_contents.get());

  WidgetAutoclosePtr widget_1(CreateTopLevelPlatformWidget());
  View* contents_view_1 = widget_1->GetContentsView();
  WebTelegram* added_web_view = contents_view_1->AddChildView(std::move(web_view));

  // After being added to the widget hierarchy the holder's NativeViewAccessible
  // should match that of the web view's parent view.
  EXPECT_EQ(added_web_view->parent()->GetNativeViewAccessible(),
            added_web_view->holder()->GetParentAccessible());

  WidgetAutoclosePtr widget_2(CreateTopLevelPlatformWidget());
  View* contents_view_2 = widget_2->GetContentsView();

  // Reparent the web view.
  added_web_view = contents_view_2->AddChildView(
      contents_view_1->RemoveChildViewT(added_web_view));

  // After reparenting the holder's NativeViewAccessible should match that of
  // the web view's new parent view.
  EXPECT_EQ(added_web_view->parent()->GetNativeViewAccessible(),
            added_web_view->holder()->GetParentAccessible());
}

// This tests that we don't crash if WebTelegram doesn't have a Widget or a
// Webcontents. https://crbug.com/1191999
TEST_F(WebTelegramUnitTest, ChangeAXMode) {
  // Case 1: WebTelegram has a Widget and no WebContents.
  SetAXMode(ui::AXMode::kFirstModeFlag);

  // Case 2: WebTelegram has no Widget and a WebContents.
  View* contents_view = top_level_widget()->GetContentsView();
  contents_view->RemoveChildView(web_view());
  const std::unique_ptr<content::WebContents> web_contents =
      CreateWebContents();
  web_view()->SetWebContents(web_contents.get());

  SetAXMode(ui::AXMode::kFirstModeFlag);

  // No crash.
}

// Tests to make sure the WebTelegram clears away the reference to its hosted
// WebContents object when its deleted.
TEST_F(WebTelegramUnitTest, WebTelegramClearsWebContentsOnDestruction) {
  std::unique_ptr<content::WebContents> web_contents = CreateWebContents();
  web_view()->SetWebContents(web_contents.get());
  EXPECT_EQ(web_contents.get(), web_view()->web_contents());
  web_contents.reset();
  EXPECT_EQ(nullptr, web_view()->web_contents());
}

}  // namespace views
