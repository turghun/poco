//
// GridLayout.cpp
//
// $Id: //poco/Main/WebWidgets/src/GridLayout.cpp#6 $
//
// Library: WebWidgets
// Package: Layouts
// Module:  GridLayout
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#include "Poco/WebWidgets/GridLayout.h"


namespace Poco {
namespace WebWidgets {


GridLayout::GridLayout(std::size_t colCnt, std::size_t rowCnt):
	Layout(typeid(GridLayout)),
	_colCnt(colCnt),
	_rowCnt(rowCnt),
	_horPadding(0),
	_vertPadding(0),
	_lastIdx(0)
{
	poco_assert (_colCnt > 0);
	if (rowCnt > 0)
		resize(rowCnt-1); //resize takes an index
}


GridLayout::~GridLayout()
{
}


View::Ptr GridLayout::get(std::size_t rowCnt, std::size_t colCnt) const
{
	std::size_t idx = pos(rowCnt, colCnt);
	if (idx < size())
		return views().at(idx);
	return View::Ptr();
}


void GridLayout::set(View::Ptr pView, std::size_t rowCnt, std::size_t colCnt)
{
	resize(rowCnt);
	_lastIdx = pos(rowCnt, colCnt);
	rejectChild(views()[_lastIdx]);
	views()[_lastIdx] = pView;
	adoptChild(pView);
	++_lastIdx;
}


void GridLayout::resize(std::size_t rowCnt)
{
	if (_rowCnt != 0 && rowCnt >= _rowCnt)
		throw Poco::RangeException("Maximum Grid size exceeded");

	if (size() < (rowCnt+1)*_colCnt)
		views().resize((rowCnt+1)*_colCnt, View::Ptr());
}


void GridLayout::minimize()
{
	ContainerView::ViewVec& vec = views();
	if (vec.empty())
		return;
	const std::size_t pos = vec.size() - 1;
	std::size_t i = pos;
	for (; i >= 0; --i)
	{
		if (vec[i])
			break;
	}
	if (i < pos)
	{
		// pos to cnt
		++i;
		// only truncate complete lines
		resize((i+_colCnt-1)/_colCnt);
	}
}


} } // namespace Poco::WebWidgets