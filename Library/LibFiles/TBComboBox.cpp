// ToolBar Combo Box


#include "stdafx.h"
#include "TBComboBox.h"


TBComboBox* TBComboBox::get(int id) {

  try {return (TBComboBox*) GetByCmd((uint) id);}
  catch (...) {return 0;}
  }



int TBComboBox::findExact(String& s) {
int n = GetCount();
int i;

  for (i = 0; i < n; i++) if (s == GetItem(i)) return i;

  return -1;
  }

