/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/StyleSheetList.h"

#include "mozilla/dom/StyleSheetListBinding.h"
#include "nsINode.h"

namespace mozilla {
namespace dom {

NS_IMPL_CYCLE_COLLECTION_WRAPPERCACHE_0(StyleSheetList)

NS_INTERFACE_MAP_BEGIN_CYCLE_COLLECTION(StyleSheetList)
  NS_WRAPPERCACHE_INTERFACE_MAP_ENTRY
  NS_INTERFACE_MAP_ENTRY(nsIMutationObserver)
  NS_INTERFACE_MAP_ENTRY(nsISupports)
NS_INTERFACE_MAP_END

NS_IMPL_CYCLE_COLLECTING_ADDREF(StyleSheetList)
NS_IMPL_CYCLE_COLLECTING_RELEASE(StyleSheetList)

/* virtual */ JSObject*
StyleSheetList::WrapObject(JSContext* aCx, JS::Handle<JSObject*> aGivenProto)
{
  return StyleSheetListBinding::Wrap(aCx, this, aGivenProto);
}

void
StyleSheetList::NodeWillBeDestroyed(const nsINode* aNode)
{
  mDocumentOrShadowRoot = nullptr;
}

StyleSheetList::StyleSheetList(DocumentOrShadowRoot& aScope)
  : mDocumentOrShadowRoot(&aScope)
{
  mDocumentOrShadowRoot->AsNode().AddMutationObserver(this);
}

StyleSheetList::~StyleSheetList()
{
  if (mDocumentOrShadowRoot) {
    mDocumentOrShadowRoot->AsNode().RemoveMutationObserver(this);
  }
}

} // namespace dom
} // namespace mozilla
