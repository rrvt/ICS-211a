// Global Options


#include "stdafx.h"
#include "Options.h"
#include "IniFile.h"



static TCchar* Section          = _T("Options");
static TCchar* LogOrientKey     = _T("LogOrient");
static TCchar* RstrOrientKey    = _T("RstrOrient");
static TCchar* MbrInfoOrientKey = _T("MbrInfoOrient");
static TCchar* TopMarginKey     = _T("TopMargin");
static TCchar* LeftMarginKey    = _T("LeftMargin");
static TCchar* RightMarginKey   = _T("RightMargin");
static TCchar* BotMarginKey     = _T("BotMargin");

Options options;


void Options::operator() () {
OptionsDlg dlg;

  dlg.logOrtn     = logOrient     == Portrait ? PortraitKey : LandscapeKey;
  dlg.rstrOrtn    = rstrOrient    == Portrait ? PortraitKey : LandscapeKey;
  dlg.mbrInfoOrtn = mbrInfoOrient == Portrait ? PortraitKey : LandscapeKey;

  dlg.topMargin   = topMargin;
  dlg.leftMargin  = leftMargin;
  dlg.rightMargin = rightMargin;
  dlg.botMargin   = botMargin;

  if (dlg.DoModal() == IDOK) {
    logOrient     = getOrientn(dlg.logOrtn);
    rstrOrient    = getOrientn(dlg.rstrOrtn);
    mbrInfoOrient = getOrientn(dlg.mbrInfoOrtn);

    topMargin    = dlg.topMargin;
    leftMargin   = dlg.leftMargin;
    rightMargin  = dlg.rightMargin;
    botMargin    = dlg.botMargin;

    store();
    }
  }


void Options::load() {
String excel;

  logOrient     = (PrtrOrient) iniFile.readInt(Section, LogOrientKey,     Portrait);
  rstrOrient    = (PrtrOrient) iniFile.readInt(Section, RstrOrientKey,    Portrait);
  mbrInfoOrient = (PrtrOrient) iniFile.readInt(Section, MbrInfoOrientKey, Portrait);

  iniFile.readString(Section, TopMarginKey,   topMargin,   _T("3.0"));
  iniFile.readString(Section, LeftMarginKey,  leftMargin,  _T("3.0"));
  iniFile.readString(Section, RightMarginKey, rightMargin, _T("3.0"));
  iniFile.readString(Section, BotMarginKey,   botMargin,   _T("3.0"));
  }


void Options::store() {

  iniFile.writeInt(   Section, LogOrientKey,     logOrient);
  iniFile.writeInt(   Section, RstrOrientKey,    rstrOrient);
  iniFile.writeInt(   Section, MbrInfoOrientKey, mbrInfoOrient);

  iniFile.writeString(Section, TopMarginKey,   topMargin);
  iniFile.writeString(Section, LeftMarginKey,  leftMargin);
  iniFile.writeString(Section, RightMarginKey, rightMargin);
  iniFile.writeString(Section, BotMarginKey,   botMargin);
  }

