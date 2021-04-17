// Global Options


#pragma once
#include "CScrView.h"
#include "OptionsDlg.h"


struct Options {
PrtrOrient logOrient;
PrtrOrient rstrOrient;
PrtrOrient mbrInfoOrient;

String     topMargin;
String     leftMargin;
String     rightMargin;
String     botMargin;

  Options() : logOrient(Portrait), rstrOrient(Portrait), mbrInfoOrient(Portrait),
                                    topMargin(0.0), leftMargin(0.0), rightMargin(0.0), botMargin(0.0) { }
 ~Options() { }

  void operator() ();

  void load();
  void store();

  void setLogOrient( PrtrOrient v) {logOrient     = v; store();}
  void setRstrOrient(PrtrOrient v) {rstrOrient    = v; store();}
  void setInfoOrient(PrtrOrient v) {mbrInfoOrient = v; store();}

private:

  PrtrOrient getOrientn(CString& cs) {String s = cs;   return s == LandscapeKey ? Landscape : Portrait;}
  };


extern Options options;

