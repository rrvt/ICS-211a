// Member Information


#pragma once
#include "CSVRcdB.h"
#include "CSVRcdsT.h"

class ICS_211aView;
class Display;


struct MemberInfo : CSVRcdB {
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

  virtual void    put(TCchar* p);

  virtual String* get();

  void copy(MemberInfo& mi) {
    firstName   = mi.firstName;   lastName = mi.lastName;
    badgeNumber = mi.badgeNumber; callSign = mi.callSign;
    }
  };


typedef RcdPtrT< MemberInfo> MemberInfoP;
typedef CSVRcdsT<MemberInfo, MemberInfoP> MembersB;
typedef CSVIterT<MemberInfo, MemberInfoP> MbrIter;


class Members : public MembersB {

public:

  Members() { }
 ~Members() { }

  bool isLoaded() {return data.end() > 0;}

  MemberInfo* find(String& callSign);
  };


extern Members members;

