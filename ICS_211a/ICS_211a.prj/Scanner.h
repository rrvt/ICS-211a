// Scanner Interface


#pragma once
#include "_CoreScanner_i.h"
#include "_CoreScanner.h"
//#include "Sink.h"


//---- Scanner types ------//
#define SCANNER_TYPES_ALL       1
#define SCANNER_TYPES_SNAPI     2
#define SCANNER_TYPES_SSI       3
#define SCANNER_TYPES_IBMHID    6
#define SCANNER_TYPES_NIXMODB   7
#define SCANNER_TYPES_HIDKB     8
#define SCANNER_TYPES_IBMTT     9
#define SCANNER_TYPES_IBM      10
#define SCANNER_TYPES_SSI_BT   11
#define SCANNER_TYPES_OPOS     13


//---- Total number of scanner types ----//
#define TOTAL_SCANNER_TYPES     (SCANNER_TYPES_OPOS)


class Scanner : public CCmdTarget {

bool          coInit;
ICoreScanner* iCore;
LPUNKNOWN     sinkUnknown;
DWORD         cookie;
bool          opened;
bool          idFound;

public:

String version;
String scannerID;
String xmlOutput;

  Scanner() : iCore(0), opened(false) {coInit = CoInitialize(0) == S_OK;}

 ~Scanner() {CoUninitialize();}

  bool open(SHORT ScannerTypes[TOTAL_SCANNER_TYPES], SHORT noTypes, long& status);
  bool reboot(     long& status);
  bool getScanners(long& status);
  bool enableScan( long& status);
  bool disableScan(long& status);
  bool getTopology(long& status);
  bool changeMode(TCchar* mode, long& status);
  bool changeLights(int code, long& status);
  bool registerEvent(long& status);
  bool unregisterEvent(long& status);

  bool getVersion(long& status);

private:

  bool    execute(LONG opcode, CComBSTR& inXML, LONG& status);
  String& getInArgID();
  String& getIDarg();
  String& getBoolArg(bool arg);
  String& getIntArg(int arg);
  bool    createInstance();
  bool    dspError(TCchar* title, long status);
  };


extern Scanner scanner;
