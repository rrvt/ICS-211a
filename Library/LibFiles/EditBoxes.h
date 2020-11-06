// Edit Box -- a child of CEdit
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.


#pragma once
#include "EditBox.h"
#include "ExpandableP.h"
#include "IterT.h"



typedef RcdPtrT<EditBox> EditBoxP;
class EditBoxes;
typedef IterT<EditBoxes, EditBox> BxsIter;


class EditBoxes {

CWnd*                             view;
ExpandableP<EditBox, EditBoxP, 2> boxes;

public:

  EditBoxes() : view(0) {}
 ~EditBoxes() { }

  void    setView(CWnd* v) {view = v;}

  void    create(int vPos, int editBoxX, int x, int y, int width, int height);

  void    set(      int x, String& s, BoxType bt) {EditBox* eb = datum(x); if (eb) eb->set(s, bt);}
  void    setFocus( int x, bool home = false)     {EditBox* eb = datum(x); if (eb) eb->setFocus(home);}
//  void    setHome(  int x)              {EditBox* eb = datum(x); if (eb) eb->setHome();}

  void    getLine(  int x, String& line) {EditBox* eb = datum(x); if (eb) eb->getLine(line);}

  int     getVertPos(int x)            {EditBox* eb = datum(x); return eb ? eb->getVertPos() : 0;}
  void    getRect(   int x, RECT& rect) {
            EditBox* eb = datum(x);
            if (eb) eb->getRect(rect);   else rect.top = rect.bottom = rect.left = rect.right = 0;
            }

  void    getWindowPos(int x, int* xx, int* yy)
            {EditBox* eb = datum(x); if (eb) eb->getWindowPos(getHWND(x), xx, yy); else *xx = *yy = 0;}

  HWND    getHWND(int x) {EditBox* eb = datum(x); return eb ? eb->m_hWnd : 0;}

  String getInitialVal(int x) {EditBox* eb = datum(x); return eb ? eb->initialVal : _T("");}

  BoxType getBoxType(int x) {EditBox* eb = datum(x); return eb ? eb->boxType : NilBox;}
  int     count() {return nData();}

  void    deleteBox(int x) {removeDatum(x);}

private:

  // returns either a pointer to data (or datum) at index i in array or zero

  EditBox* datum(int i) {return 0 <= i && i < nData() ? boxes[i].p : 0;}       // or data[i].p

  int      nData()      {return boxes.end();}                   // returns number of data items in array

  void     removeDatum(int i) {if (0 <= i && i < nData()) boxes.del(i);}

  friend typename BxsIter;
  };


extern EditBoxes editBoxes;


#if 0
class EditBoxX {
int x;

public:

  EditBoxX() {x = -1; get();}
 ~EditBoxX() { }

  operator int() {if (x < 0) get();  return x;}

  void get()   {if (x < 0) x = 1/*editBoxes.get()*/;}

  void set(String& initialVal, BoxType bt)   {get(); editBoxes.set(x, initialVal, bt);}

  void setFocus();// {if (x > -1) editBoxes.setFocus(x); docView.scrollToPos();}

  int  getVertPos()                   {return editBoxes.getVertPos(x);}
  void getRect(RECT& rect)            {editBoxes.getRect(x, rect);}
  void getWindowPos(int* xx, int* yy) {editBoxes.getWindowPos(x, xx, yy);}

  void getLine(String& line) {if (x < 0) get(); editBoxes.getLine(x, line);}

//  void deleteBox() {if (x >= 0) editBoxes.deleteBox(x);}

  HWND getHWND() {return editBoxes.getHWND(x);}
  };
#endif



#if 0
struct EditBoxP {
EditBox* p;


  EditBoxP() {NewAlloc(EditBox); p = AllocNode;}
 ~EditBoxP() {NewAlloc(EditBox); FreeNode(p); p = 0;}

  EditBoxP(EditBoxP& fd) {p = fd.p;}

  EditBoxP& operator= (EditBoxP& fd) {p = fd.p; return *this;}
  };
#endif

