// Font Manager
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.


#pragma once
#include "ExpandableP.h"
#include "IterT.h"

/*
typedef struct tagLOGFONTW {
  LONG  lfHeight;
  LONG  lfWidth;
  LONG  lfEscapement;
  LONG  lfOrientation;
  LONG  lfWeight;
  BYTE  lfItalic;
  BYTE  lfUnderline;
  BYTE  lfStrikeOut;
  BYTE  lfCharSet;
  BYTE  lfOutPrecision;
  BYTE  lfClipPrecision;
  BYTE  lfQuality;
  BYTE  lfPitchAndFamily;
  WCHAR lfFaceName[LF_FACESIZE];
  } LOGFONTW, *PLOGFONTW, *NPLOGFONTW, *LPLOGFONTW;

*/


struct FontAttr {
CDC*   dc;
String face;
double sz;
bool   bold;
bool   italic;
bool   underline;
bool   strikeout;

  FontAttr() {clear();}
  FontAttr(FontAttr& fd) {copy(fd);}
 ~FontAttr() {clear();}

  void clear() {dc = 0; sz = 0; bold = italic = underline = strikeout = false; face.clear();}

  FontAttr& operator= (FontAttr& fd) {copy(fd); return *this;}

private:

  void copy(FontAttr& src) {
    dc     = src.dc;       face      = src.face;        sz        = src.sz;       bold = src.bold;
    italic = src.italic;   underline = src.underline;   strikeout = src.strikeout;
    }
  };



typedef RcdPtrT<FontAttr> FontAttrP;
class FontMgr;
typedef IterT<FontMgr, FontAttr> FntIter;


class FontMgr {
LOGFONT                             systemFont;
FontAttr                            baseAttr;
bool                                sysFontIntlzd;

double                              scale;

int                                 stkX;
ExpandableP<FontAttr, FontAttrP, 4> stk;

public:

  FontMgr() : sysFontIntlzd(false), stkX(0), scale(0) { }
 ~FontMgr();

  void clear();

  void initialize(TCchar* face, double fontSize, CDC* dc);
  void reset();

  void pop();
  void setFace(TCchar* face);
  void setSize(double  size);
  void setBold();
  void setItalic();
  void setUnderLine();
  void setStrikeOut();

  FontAttr& getCurAttr()   {return getAttr(stkX);}
  FontAttr& getAttr(int i) {return stk.getData(i);}

  double    getScale() {return scale;}
  void      setScale(double scl) {scale = scl;}

private:

  void      initializeSysFont(CFont& font, CDC* dc);
  FontAttr& push();
  void      update(FontAttr& attr, TCchar* fn);
  void      select(CFont& font, CDC* dc);
  void      deleteObject(CFont& font);
  bool      createPointFont(CDC* dc,TCchar* face,  double size, CFont& font);

  void      createFailed(TCchar* fn);
  };


