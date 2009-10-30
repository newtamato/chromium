// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_GLUE_DOM_OPERATIONS_H__
#define WEBKIT_GLUE_DOM_OPERATIONS_H__

#include <string>
#include <map>

#include "base/gfx/size.h"
#include "googleurl/src/gurl.h"
#include "webkit/glue/password_form_dom_manager.h"

namespace WebKit {
class WebView;
}

struct FormData;

// A collection of operations that access the underlying WebKit DOM directly.
namespace webkit_glue {

// Fill in a form identified by form |data|.
bool FillForm(WebKit::WebView* view, const FormData& data);

// Fill matching password forms and trigger autocomplete in the case of multiple
// matching logins.
void FillPasswordForm(WebKit::WebView* view,
                      const PasswordFormDomManager::FillData& data);

// Structure for storage the result of getting all savable resource links
// for current page. The consumer of the SavableResourcesResult is responsible
// for keeping these pointers valid for the lifetime of the
// SavableResourcesResult instance.
struct SavableResourcesResult {
  // vector which contains all savable links of sub resource.
  std::vector<GURL>* resources_list;
  // vector which contains corresponding all referral links of sub resource,
  // it matched with links one by one.
  std::vector<GURL>* referrers_list;
  // vector which contains all savable links of main frame and sub frames.
  std::vector<GURL>* frames_list;

  // Constructor.
  SavableResourcesResult(std::vector<GURL>* resources_list,
                         std::vector<GURL>* referrers_list,
                         std::vector<GURL>* frames_list)
      : resources_list(resources_list),
        referrers_list(referrers_list),
        frames_list(frames_list) { }

 private:
  DISALLOW_EVIL_CONSTRUCTORS(SavableResourcesResult);
};

// Get all savable resource links from current webview, include main frame
// and sub-frame. After collecting all savable resource links, this function
// will send those links to embedder. Return value indicates whether we get
// all saved resource links successfully.
bool GetAllSavableResourceLinksForCurrentPage(WebKit::WebView* view,
    const GURL& page_url, SavableResourcesResult* savable_resources_result,
    const char** savable_schemes);

// Structure used when installing a web page as an app. Populated via
// GetApplicationInfo.
struct WebApplicationInfo {
  struct IconInfo {
    GURL url;
    int width;
    int height;
  };

  // Title of the application. This is set from the meta tag whose name is
  // 'application-name'.
  string16 title;

  // Description of the application. This is set from the meta tag whose name
  // is 'description'.
  string16 description;

  // URL for the app. This is set from the meta tag whose name is
  // 'application-url'.
  GURL app_url;

  // Set of available icons. This is set for all link tags whose rel=icon. Only
  // icons that have a non-zero (width and/or height) are added.
  std::vector<IconInfo> icons;
};

// Parses the icon's size attribute as defined in the HTML 5 spec. Returns true
// on success, false on errors. On success either all the sizes specified in
// the attribute are added to sizes, or is_any is set to true.
//
// You shouldn't have a need to invoke this directly, it's public for testing.
bool ParseIconSizes(const string16& text,
                    std::vector<gfx::Size>* sizes,
                    bool* is_any);

// Gets the application info for the specified page. See the description of
// WebApplicationInfo for details as to where each field comes from.
void GetApplicationInfo(WebKit::WebView* view, WebApplicationInfo* app_info);

// Invokes pauseAnimationAtTime on the AnimationController associated with the
// |view|s main frame.
// This is used by test shell.
bool PauseAnimationAtTimeOnElementWithId(WebKit::WebView* view,
                                         const std::string& animation_name,
                                         double time,
                                         const std::string& element_id);

// Invokes pauseTransitionAtTime on the AnimationController associated with the
// |view|s main frame.
// This is used by test shell.
bool PauseTransitionAtTimeOnElementWithId(WebKit::WebView* view,
                                          const std::string& property_name,
                                          double time,
                                          const std::string& element_id);

// Returns true if the element with |element_id| as its id has autocomplete
// on.
bool ElementDoesAutoCompleteForElementWithId(WebKit::WebView* view,
                                             const std::string& element_id);

// Returns the number of animations currently running.
int NumberOfActiveAnimations(WebKit::WebView* view);

}  // namespace webkit_glue

#endif  // WEBKIT_GLUE_DOM_OPERATIONS_H__
