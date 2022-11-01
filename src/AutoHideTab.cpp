/*******************************************************************************
** Qt Advanced Docking System
** Copyright (C) 2017 Uwe Kindler
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public
** License along with this library; If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/


//============================================================================
/// \file   DockWidgetTab.h
/// \author Syarif Fakhri
/// \date   05.09.2022
/// \brief  Implementation of CDockWidgetSideTab class
//============================================================================

//============================================================================
//                                   INCLUDES
//============================================================================
#include <AutoHideDockContainer.h>
#include <AutoHideSideBar.h>
#include <AutoHideTab.h>
#include <QBoxLayout>

#include "DockAreaWidget.h"
#include "DockManager.h"

#include "DockWidget.h"

namespace ads
{
/**
 * Private data class of CDockWidgetTab class (pimpl)
 */
struct AutoHideTabPrivate
{
    CAutoHideTab* _this;
    CDockWidget* DockWidget = nullptr;
    CAutoHideSideBar* SideBar = nullptr;
	Qt::Orientation Orientation{Qt::Vertical};

	/**
	 * Private data constructor
	 */
	AutoHideTabPrivate(CAutoHideTab* _public);
}; // struct DockWidgetTabPrivate


//============================================================================
AutoHideTabPrivate::AutoHideTabPrivate(CAutoHideTab* _public) :
	_this(_public)
{

}


//============================================================================
void CAutoHideTab::setSideBar(CAutoHideSideBar* SideTabBar)
{
	d->SideBar = SideTabBar;
}


//============================================================================
void CAutoHideTab::removeFromSideBar()
{
	if (d->SideBar == nullptr)
	{
		return;
	}
	d->SideBar->removeSideTab(this);
    setSideBar(nullptr);
}

//============================================================================
CAutoHideTab::CAutoHideTab(QWidget* parent) :
	Super(parent),
	d(new AutoHideTabPrivate(this))
{
	setAttribute(Qt::WA_NoMousePropagation);
	setFocusPolicy(Qt::NoFocus);
}


//============================================================================
CAutoHideTab::~CAutoHideTab()
{
	qDebug() << "~CDockWidgetSideTab()";
	delete d;
}


//============================================================================
void CAutoHideTab::updateStyle()
{
    internal::repolishStyle(this, internal::RepolishDirectChildren);
	update();
}


//============================================================================
SideBarLocation CAutoHideTab::sideTabBarArea() const
{
    if (d->SideBar)
	{
        return d->SideBar->sideBarArea();
	}

	return Left;
}


//============================================================================
void CAutoHideTab::setOrientation(Qt::Orientation Orientation)
{
	d->Orientation = Orientation;
	CPushButton::setButtonOrientation((Qt::Horizontal == Orientation)
		? CPushButton::Horizontal : CPushButton::VerticalTopToBottom);
	updateStyle();
}


//============================================================================
Qt::Orientation CAutoHideTab::orientation() const
{
	return d->Orientation;
}


//============================================================================
void CAutoHideTab::updateOrientationForArea(SideBarLocation area)
{
	setOrientation((area == Bottom || area == Top) ? Qt::Horizontal : Qt::Vertical);

	if (icon().isNull())
	{
		return;
	}

	bool IconOnly = false;
	switch (area)
	{
	case SideBarLocation::Left:
		 IconOnly = CDockManager::testConfigFlag(CDockManager::LeftSideBarIconOnly);
		 break;

	case SideBarLocation::Right:
		 IconOnly = CDockManager::testConfigFlag(CDockManager::RightSideBarIconOnly);
		 break;

	case SideBarLocation::Top:
		 IconOnly = CDockManager::testConfigFlag(CDockManager::BottomSideBarIconOnly);
		 break;

	case SideBarLocation::Bottom:
		 IconOnly = CDockManager::testConfigFlag(CDockManager::TopSideBarIconOnly);
		 break;
	}

	if (IconOnly)
	{
		setText("");
		setOrientation(Qt::Horizontal);
	}
}


//============================================================================
bool CAutoHideTab::isActiveTab() const
{
	if (d->DockWidget && d->DockWidget->autoHideDockContainer())
	{
		return d->DockWidget->autoHideDockContainer()->isVisible();
	}

	return false;
}


//============================================================================
CDockWidget* CAutoHideTab::dockWidget() const
{
	return d->DockWidget;
}


//============================================================================
void CAutoHideTab::setDockWidget(CDockWidget* DockWidget)
{
	if (!DockWidget)
	{
		return;
	}
	d->DockWidget = DockWidget;
	setText(DockWidget->windowTitle());
	setIcon(d->DockWidget->icon());
}

}