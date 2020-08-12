// Member Information


#pragma once
#include "Archive.h"
#include "Expandable.h"
#include "IterT.h"


struct MemberInfo {
String firstName;
String lastName;
String badgeNumber;
String callSign;

  MemberInfo() { }
  MemberInfo(MemberInfo& mi) {copy(mi);}
 ~MemberInfo() { }

  //  *s =  *p
  //  *p == *q
  //  *p >  *q
  //  *p <= *q

  MemberInfo& operator=  (MemberInfo& mi) {copy(mi); return *this;}
  bool        operator>  (MemberInfo& mi) {return callSign >  mi.callSign;}
  bool        operator<= (MemberInfo& mi) {return callSign <= mi.callSign;}
  bool        operator== (MemberInfo& mi) {return callSign == mi.callSign;}

private:

  void copy(MemberInfo& mi) {
    firstName   = mi.firstName;   lastName = mi.lastName;
    badgeNumber = mi.badgeNumber; callSign = mi.callSign;
    }
  };


struct Members;
typedef IterT<Members, MemberInfo> MbrIter;


struct Members {

bool                      loaded;
Expandable<MemberInfo, 2> data;

  Members() : loaded(false) { }
 ~Members() { }

  void        clr() {data.clr();}

  void        load(Archive& ar);
  bool        isLoaded() {return loaded;}
  void        display();

//  int         end() {return data.end();}
//  MemberInfo& operator[] (int i) {return data[i];}

  MemberInfo* find(String& fcc);

private:

  // returns either a pointer to data (or datum) at index i in array or zero
  MemberInfo* datum(int i) {return 0 <= i && i < nData() ? &data[i] : 0;}

  // returns number of data items in array
  int   nData()      {return data.end();}

  friend typename MbrIter;
  };

extern Members members;
