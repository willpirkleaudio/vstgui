// This file is part of VSTGUI. It is subject to the license terms
// in the LICENSE file found in the top-level directory of this
// distribution and at http://github.com/steinbergmedia/vstgui/LICENSE

#include "splitviewcreator.h"

#include "../../lib/csplitview.h"
#include "../detail/uiviewcreatorattributes.h"
#include "../uiattributes.h"
#include "../uiviewcreator.h"
#include "../uiviewfactory.h"

//------------------------------------------------------------------------
namespace VSTGUI {
namespace UIViewCreator {

//------------------------------------------------------------------------
SplitViewCreator::SplitViewCreator ()
{
	UIViewFactory::registerViewCreator (*this);
}

//------------------------------------------------------------------------
IdStringPtr SplitViewCreator::getViewName () const
{
	return kCSplitView;
}

//------------------------------------------------------------------------
IdStringPtr SplitViewCreator::getBaseViewName () const
{
	return kCViewContainer;
}

//------------------------------------------------------------------------
UTF8StringPtr SplitViewCreator::getDisplayName () const
{
	return "Split View";
}

//------------------------------------------------------------------------
CView* SplitViewCreator::create (const UIAttributes& attributes,
                                 const IUIDescription* description) const
{
	return new CSplitView (CRect (0, 0, 100, 100));
}

//------------------------------------------------------------------------
bool SplitViewCreator::apply (CView* view, const UIAttributes& attributes,
                              const IUIDescription* description) const
{
	auto* splitView = dynamic_cast<CSplitView*> (view);
	if (!splitView)
		return false;

	int32_t width;
	if (attributes.getIntegerAttribute (kAttrSeparatorWidth, width))
		splitView->setSeparatorWidth (width);
	const auto* attr = attributes.getAttributeValue (kAttrOrientation);
	if (attr)
	{
		if (*attr == strHorizontal)
		{
			splitView->setStyle (CSplitView::kHorizontal);
		}
		else
		{
			splitView->setStyle (CSplitView::kVertical);
		}
	}
	attr = attributes.getAttributeValue (kAttrResizeMethod);
	if (attr)
	{
		if (*attr == kFirst)
		{
			splitView->setResizeMethod (CSplitView::kResizeFirstView);
		}
		if (*attr == kSecond)
		{
			splitView->setResizeMethod (CSplitView::kResizeSecondView);
		}
		else if (*attr == kLast)
		{
			splitView->setResizeMethod (CSplitView::kResizeLastView);
		}
		else if (*attr == kAll)
		{
			splitView->setResizeMethod (CSplitView::kResizeAllViews);
		}
	}

	return true;
}

//------------------------------------------------------------------------
bool SplitViewCreator::getAttributeNames (StringList& attributeNames) const
{
	attributeNames.emplace_back (kAttrOrientation);
	attributeNames.emplace_back (kAttrResizeMethod);
	attributeNames.emplace_back (kAttrSeparatorWidth);
	return true;
}

//------------------------------------------------------------------------
auto SplitViewCreator::getAttributeType (const string& attributeName) const -> AttrType
{
	if (attributeName == kAttrOrientation)
		return kListType;
	if (attributeName == kAttrResizeMethod)
		return kListType;
	if (attributeName == kAttrSeparatorWidth)
		return kIntegerType;
	return kUnknownType;
}

//------------------------------------------------------------------------
bool SplitViewCreator::getAttributeValue (CView* view, const string& attributeName,
                                          string& stringValue, const IUIDescription* desc) const
{
	auto* splitView = dynamic_cast<CSplitView*> (view);
	if (!splitView)
		return false;
	if (attributeName == kAttrSeparatorWidth)
	{
		stringValue =
		    UIAttributes::integerToString (static_cast<int32_t> (splitView->getSeparatorWidth ()));
		return true;
	}
	if (attributeName == kAttrOrientation)
	{
		stringValue =
		    splitView->getStyle () == CSplitView::kHorizontal ? strHorizontal : strVertical;
		return true;
	}
	if (attributeName == kAttrResizeMethod)
	{
		switch (splitView->getResizeMethod ())
		{
			case CSplitView::kResizeFirstView:
			{
				stringValue = kFirst;
				return true;
			}
			case CSplitView::kResizeSecondView:
			{
				stringValue = kSecond;
				return true;
			}
			case CSplitView::kResizeLastView:
			{
				stringValue = kLast;
				return true;
			}
			case CSplitView::kResizeAllViews:
			{
				stringValue = kAll;
				return true;
			}
		}
	}
	return false;
}

//------------------------------------------------------------------------
bool SplitViewCreator::getPossibleListValues (const string& attributeName,
                                              ConstStringPtrList& values) const
{
	if (attributeName == kAttrOrientation)
	{
		return getStandardAttributeListValues (kAttrOrientation, values);
	}
	else if (attributeName == kAttrResizeMethod)
	{
		values.emplace_back (&kFirst);
		values.emplace_back (&kSecond);
		values.emplace_back (&kLast);
		values.emplace_back (&kAll);
		return true;
	}
	return false;
}
//------------------------------------------------------------------------
} // UIViewCreator
} // VSTGUI
