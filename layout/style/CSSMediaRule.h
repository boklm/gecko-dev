/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_CSSMediaRule_h
#define mozilla_dom_CSSMediaRule_h

#include "mozilla/css/GroupRule.h"
#include "mozilla/ServoBindingTypes.h"

namespace mozilla {
namespace dom {

class CSSMediaRule final : public css::ConditionRule
{
public:
  CSSMediaRule(RefPtr<RawServoMediaRule> aRawRule,
               uint32_t aLine, uint32_t aColumn);

  NS_DECL_ISUPPORTS_INHERITED
  NS_DECL_CYCLE_COLLECTION_CLASS_INHERITED(CSSMediaRule, css::ConditionRule)

  void SetStyleSheet(StyleSheet* aSheet) override;

#ifdef DEBUG
  void List(FILE* out = stdout, int32_t aIndent = 0) const final;
#endif

  RawServoMediaRule* Raw() const { return mRawRule; }

  // WebIDL interface
  uint16_t Type() const override { return CSSRuleBinding::MEDIA_RULE; }
  // WebIDL interface
  void GetCssText(nsAString& aCssText) const final;
  void GetConditionText(nsAString& aConditionText) final;
  void SetConditionText(const nsAString& aConditionText,
                        ErrorResult& aRv) final;
  dom::MediaList* Media();

  size_t SizeOfIncludingThis(mozilla::MallocSizeOf aMallocSizeOf)
    const override;

  JSObject* WrapObject(JSContext* aCx,
                       JS::Handle<JSObject*> aGivenProto) override;

private:
  virtual ~CSSMediaRule();

  RefPtr<RawServoMediaRule> mRawRule;
  RefPtr<dom::MediaList> mMediaList;
};

} // namespace dom
} // namespace mozilla

#endif // mozilla_dom_CSSMediaRule_h
