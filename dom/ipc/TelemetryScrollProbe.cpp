/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "TelemetryScrollProbe.h"

#include "nsIURI.h"             // for nsIURI
#include "TabChild.h"           // for TabChildGlobal, TabChildBase
#include "mozilla/Telemetry.h"  // for mozilla::Telemetry
#include "mozilla/dom/Event.h"  // for Event

namespace mozilla {
namespace dom {

/* static */ void
TelemetryScrollProbe::Create(TabChildGlobal* aWebFrame)
{
  nsWeakPtr webNav = do_GetWeakReference(aWebFrame->mTabChild->WebNavigation());
  RefPtr<TelemetryScrollProbe> probe = new TelemetryScrollProbe(webNav);

  aWebFrame->AddEventListener(NS_LITERAL_STRING("pagehide"), probe, true);
}

already_AddRefed<nsIWebNavigation>
TelemetryScrollProbe::GetWebNavigation() const
{
  nsCOMPtr<nsIWebNavigation> webNav = do_QueryReferent(mWebNav);
  return webNav.forget();
}

already_AddRefed<nsIDocument>
TelemetryScrollProbe::GetDocument() const
{
  nsCOMPtr<nsIDocument> result;
  if (nsCOMPtr<nsIWebNavigation> webNav = GetWebNavigation()) {
    webNav->GetDocument(getter_AddRefs(result));
  }
  return result.forget();
}

already_AddRefed<nsIPresShell>
TelemetryScrollProbe::GetPresShell() const
{
  nsCOMPtr<nsIPresShell> result;
  if (nsCOMPtr<nsIDocument> doc = GetDocument()) {
    result = doc->GetShell();
  }
  return result.forget();
}

bool
TelemetryScrollProbe::ShouldIgnore(Event* aEvent) const
{
  RefPtr<nsIDocument> document = GetDocument();

  return !document ||
         aEvent->GetTarget() != document ||
         nsContentUtils::IsSystemPrincipal(document->NodePrincipal());
}

NS_IMPL_ISUPPORTS(TelemetryScrollProbe, nsIDOMEventListener)

NS_IMETHODIMP
TelemetryScrollProbe::HandleEvent(Event* aEvent)
{
  RefPtr<nsIPresShell> presShell = GetPresShell();

  if (!presShell || ShouldIgnore(aEvent)) {
    return NS_OK;
  }

  RefPtr<nsPresContext> presContext = presShell->GetPresContext();

  nscoord maxAppUnits = presContext->TelemetryScrollMaxY();
  nscoord totalAppUnits = presContext->TelemetryScrollTotalY();

  float maxCSSPixels = nsPresContext::AppUnitsToFloatCSSPixels(maxAppUnits);
  float totalCSSPixels = nsPresContext::AppUnitsToFloatCSSPixels(totalAppUnits);

  mozilla::Telemetry::Accumulate(mozilla::Telemetry::TOTAL_SCROLL_Y, totalCSSPixels);
  mozilla::Telemetry::Accumulate(mozilla::Telemetry::PAGE_MAX_SCROLL_Y, maxCSSPixels);

  return NS_OK;
}

} // namespace dom
} // namespace mozilla
