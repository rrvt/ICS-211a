// Show Manager -- Parent class for DsplyMgr and PrintMgr


#pragma once
#include "stdafx.h"
#include "ShowMgr.h"


ShowMgr::ShowMgr(CScrView& view, NotePad& notePad) : vw(view), npd(notePad), dev(notePad),
                                                     font(_T("Arial")), fontSize(120),
                                                     leftMargin(1.0), rightMargin(1.0),
                                                     topMargin(1.0), botMargin(1.0) { }




void ShowMgr::setMgns(double left, double top, double right, double bot)
                            {leftMargin = left; topMargin = top;  rightMargin = right;  botMargin = bot;}

