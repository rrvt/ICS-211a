// Scanner Interface


#include "stdafx.h"
#include "Scanner.h"
#include "NotePad.h"
#include <afxctl.h>


//--------- Command opcodes --------//
enum ScannerOp {

  ERROR_OPCODE                       = -1,

  GET_VERSION                        =  0x3E8,  //1000
  REGISTER_FOR_EVENTS                =  0x3E9,  //1001
  UNREGISTER_FOR_EVENTS              =  0x3EA,  //1002

  CLAIM_DEVICE                       =  0x5DC,  //1500
  RELEASE_DEVICE                     =  0x5DD,  //1501

  DEVICE_ABORT_MACROPDF              =  0x7D0,  //2000
  DEVICE_ABORT_UPDATE_FIRMWARE       =  0x7D1,  //2001
  DEVICE_AIM_OFF                     =  0x7D2,  //2002
  DEVICE_AIM_ON                      =  0x7D3,  //2003
  DEVICE_ENTER_LOW_POWER_MODE        =  0x7D4,  //2004
  DEVICE_FLUSH_MACROPDF              =  0x7D5,  //2005
  DEVICE_GET_PARAMETERS              =  0x7D7,  //2007
  DEVICE_GET_SCANNER_CAPABILITIES    =  0x7D8,  //2008
  DEVICE_LED_OFF                     =  0x7D9,  //2009
  DEVICE_LED_ON                      =  0x7DA,  //2010
  DEVICE_PULL_TRIGGER                =  0x7DB,  //2011
  DEVICE_RELEASE_TRIGGER             =  0x7DC,  //2012
  DEVICE_SCAN_DISABLE                =  0x7DD,  //2013
  DEVICE_SCAN_ENABLE                 =  0x7DE,  //2014
  DEVICE_SET_PARAMETER_DEFAULTS      =  0x7DF,  //2015
  DEVICE_SET_PARAMETERS              =  0x7E0,  //2016
  DEVICE_SET_PARAMETER_PERSISTANCE   =  0x7E1,  //2017
  DEVICE_BEEP_CONTROL                =  0x7E2,  //2018
  REBOOT_SCANNER                     =  0x7E3,  //2019
  DISCONNECT_BT_SCANNER              =  0x7E7,  //2023
  DEVICE_CAPTURE_IMAGE               =  0xBB8,  //3000
  DEVICE_ABORT_IMAGE_XFER            =  0xBB9,  //3001
  DEVICE_CAPTURE_BARCODE             =  0xDAC,  //3500
  DEVICE_CAPTURE_VIDEO               =  0xFA0,  //4000

  GET_PAIRING_BARCODE                =  0x3ED,  //1005

  RSM_ATTR_GETALL                    = 0x1388,  //5000
  RSM_ATTR_GET                       = 0x1389,  //5001
  RSM_ATTR_GETNEXT                   = 0x138A,  //5002
  RSM_ATTR_GETOFFSET                 = 0x138B,  //5003
  RSM_ATTR_SET                       = 0x138C,  //5004
  RSM_ATTR_STORE                     = 0x138D,  //5005
  GET_DEVICE_TOPOLOGY                = 0x138E,  //5006
  REFRESH_TOPOLOGY                   = 0x138F,  //5007
  UPDATE_DECODE_TONE                 = 0x13BA,  //5050
  ERASE_DECODE_TONE                  = 0x13BB,  //5051

  START_NEW_FIRMWARE                 = 0x1396,  //5014
  UPDATE_ATTRIB_META_FILE            = 0x1397,  //5015
  DEVICE_UPDATE_FIRMWARE             = 0x1398,  //5016
  DEVICE_UPDATE_FIRMWARE_FROM_PLUGIN = 0x1399,  //5017


  DEVICE_SET_SERIAL_PORT_SETTINGS    = 0x17D5,  //6101
  SET_ACTION                         = 0x1770,  //6000
  DEVICE_SWITCH_HOST_MODE            = 0x1838,  //6200

  //HID keyboard emulator opcodes
  KEYBOARD_EMULATOR_ENABLE           = 0x189C,  //6300
  KEYBOARD_EMULATOR_SET_LOCALE       = 0x189D,  //6301
  KEYBOARD_EMULATOR_GET_CONFIG       = 0x189E,  //6302

  //DRIVER ADF COMMANDS
  CONFIGURE_DADF                     = 0x1900,  //6400
  RESET_DADF                         = 0x1901,  //6401


  //Scale opcodes
  SCALE_READ_WEIGHT                  = 0x1b58,  //7000
  SCALE_ZERO_SCALE                   = 0X1B5A,  //7002
  SCALE_SYSTEM_RESET                 = 0X1B67,  //7015
  };


static TCchar* BInArgs = _T("<inArgs>");
static TCchar* EInArgs = _T("</inArgs>");


Scanner scanner;



bool Scanner::open(SHORT ScannerTypes[TOTAL_SCANNER_TYPES], SHORT noTypes, long& status) {
HRESULT        hr   = S_FALSE;
SAFEARRAY*     pSA  = NULL;
SAFEARRAYBOUND bound[1];

  if (opened) return true;

  if (!createInstance()) return 0;

  bound[0].lLbound   = 0;
  bound[0].cElements = 8;

  pSA = SafeArrayCreate(VT_I2, 1, bound);

  for (long i = 0; i < 8; i++) {SafeArrayPutElement(pSA, &i, &ScannerTypes[i]);}

  return opened = iCore->Open(0, pSA, noTypes, &status) == 0 && dspError(_T("Scanner::open"), status);
  }


static TCchar* BegScannerID  = _T("<scannerID>");
static int     NBegScannerID = _tcslen(BegScannerID);
static TCchar* EndScannerID  = _T("</scannerID>");



bool Scanner::getScanners(long& status) {
SHORT          NumScan = 0;
SAFEARRAY*     pSA     = NULL;
SAFEARRAYBOUND bound[1];
BSTR           outXml;
int            beg;
int            end;

  if (!opened) return false;

  bound[0].lLbound = 0;   bound[0].cElements = 255;   pSA = SafeArrayCreate(VT_I2, 1, bound);

  idFound = iCore->GetScanners(&NumScan, pSA, &outXml, &status) == 0 &&
                                                            dspError(_T("Scanner::getScanners"), status);

  xmlOutput = outXml;   beg = xmlOutput.find(BegScannerID);   end = xmlOutput.find(EndScannerID);

  if (beg < 0) {idFound = false; return false;}

  beg += NBegScannerID;

  scannerID = xmlOutput.substr(beg, end - beg);   return idFound;
  }


static TCchar*   Version  =  _T("version=\"");
static const int NVersion = _tcslen(Version);


bool Scanner::getVersion(long& status) {
CComBSTR input = _T("<inArgs></inArgs>");
int      beg;
int      end;

  if (!opened) return false;

  if (execute(GET_VERSION, input, status) && dspError(_T("Scanner::getVersion"), status)) {

    beg = xmlOutput.find(Version);

    if (beg >= 0) {
      beg += NVersion;   end = xmlOutput.find(_T("\""), beg);   version = xmlOutput.substr(beg, end-beg);

      return true;
      }
    }

  return false;
  }



bool Scanner::reboot(long& status) {
CComBSTR input = getInArgID();

  if (!opened) return false;

  return execute(REBOOT_SCANNER, input, status) && dspError(_T("Scanner::reboot"), status);
  }






bool Scanner::getTopology(long& status) {
CComBSTR input = _T("<inArgs></inArgs>");   //getInArgID();

  if (!opened) return false;

  return execute(GET_DEVICE_TOPOLOGY, input, status) && dspError(_T("Scanner::getTopology"), status);
  }


static TCchar* BCmdArgs = _T("<cmdArgs>");
static TCchar* ECmdArgs = _T("</cmdArgs>");
static TCchar* BArgStg  = _T("<arg-string>");
static TCchar* EArgStg  = _T("</arg-string>");
static TCchar* BArgInt  = _T("<arg-int>");
static TCchar* EArgInt  = _T("</arg-int>");
static TCchar* BArgBool = _T("<arg-bool>");
static TCchar* EArgBool = _T("</arg-bool>");



bool Scanner::registerEvent(long& status) {
String   s;
CComBSTR input;

  if (!opened) return false;

  s  = BInArgs;
  s += BCmdArgs;
  s += BArgInt;
  s += _T('1');
  s += EArgInt;
  s += BArgInt;
  s += _T('1');
  s += EArgInt;
  s += ECmdArgs;
  s += EInArgs;
  input = s;

  return execute(REGISTER_FOR_EVENTS, input, status) && dspError(_T("Scanner::registerEvent"), status);
  }




bool Scanner::unregisterEvent(long& status) {
String   s;
CComBSTR input;

  if (!opened) return false;

  s  = BInArgs;
  s += BCmdArgs;
  s += BArgInt;
  s += _T('6');
  s += EArgInt;
  s += BArgInt;
  s += _T("1,2,4,8,16,32");
  s += EArgInt;
  s += ECmdArgs;
  s += EInArgs;
  input = s;

  return execute(UNREGISTER_FOR_EVENTS, input, status) && dspError(_T("Scanner::registerEvent"), status);
  }


bool Scanner::changeMode(TCchar* mode, long& status) {
String   s;
CComBSTR input;

  if (!opened) return false;

  s  = BInArgs;
  s += getIDarg();
  s += BCmdArgs;
  s += BArgStg;
  s += mode;
  s += EArgStg;
  s += BArgBool;
  s += _T("FALSE");
  s += EArgBool;
  s += BArgBool;
  s += _T("FALSE");
  s += EArgBool;
  s += ECmdArgs;
  s += EInArgs;

  input = s;

  return execute(DEVICE_SWITCH_HOST_MODE, input, status) && dspError(_T("Scanner::changeMode"), status);
  }


bool Scanner::changeLights(int code, long& status) {
CComBSTR input = getIntArg(code);

  if (!opened) return false;

  return execute(SET_ACTION, input, status) && dspError(_T("Scanner::enableScan"), status);
  }


TCchar* Scanner::getIntArg(int arg) {
static String s;

  s  = BInArgs;
  s += getIDarg();
  s += BCmdArgs;
  s += BArgInt;
  s += arg;
  s += EArgInt;
  s += ECmdArgs;
  s += EInArgs;
  return s;
  }


bool Scanner::enableScan(long& status) {
CComBSTR input = getBoolArg(true);

  if (!opened) return false;

  return execute(KEYBOARD_EMULATOR_ENABLE, input, status) && dspError(_T("Scanner::enableScan"), status);
  }


bool Scanner::disableScan(long& status) {
CComBSTR input = getBoolArg(false);

  if (!opened) return false;

  return execute(KEYBOARD_EMULATOR_ENABLE, input, status) && dspError(_T("Scanner::enableScan"), status);
  }


TCchar* Scanner::getBoolArg(bool arg) {
static String s;
  s  = BInArgs;
  s += BCmdArgs;
  s += BArgBool;
  s += arg ? _T("TRUE") : _T("FALSE");
  s += EArgBool;
  s += ECmdArgs;
  s += EInArgs;
  return s;
  }


bool Scanner::execute(LONG opcode, CComBSTR& inXML, LONG& status) {
BSTR outXml;

  if (!opened) return false;

  if (iCore->ExecCommand(opcode, &inXML, &outXml, &status) == 0)
  {if (outXml) xmlOutput = outXml; else xmlOutput.clear();   return true;}

  String e; e.format(_T("Scanner::execute opcode %li"), opcode);

  xmlOutput.clear();   dspError(e, status);   return false;
  }


TCchar* Scanner::getInArgID() {static String inArg = BInArgs + getIDarg() + EInArgs;  return inArg;}


static TCchar* BScannerID = _T("<scannerID>");
static TCchar* EScannerID = _T("</scannerID>");


String& Scanner:: getIDarg() {static String idArg = BScannerID + scannerID + EScannerID;  return idArg;}




bool Scanner::createInstance() {
HRESULT hr = S_FALSE;

  if (iCore)   return true;
  if (!coInit) return false;

  //Create the Scanner COM object
  if (CoCreateInstance(CLSID_CCoreScanner, NULL, CLSCTX_ALL, IID_ICoreScanner, (void**) &iCore) != S_OK)
    iCore = 0;

  return iCore != 0;
  }



struct ErrorData {
int     code;
TCchar* desc;
};

static ErrorData errorData[] = {{116, _T("Device does not support the Opcode")},
                                {200, _T("CoreScanner is already opened")},
                                };
static const int NErrorData = noElements(errorData);


bool Scanner::dspError(TCchar* title, long status) {
int i;

  if (!status) return true;

  for (i = 0; i < NErrorData; i++) {
    ErrorData& data = errorData[i];

    if (data.code == status) {
      notePad << title << _T(" error:  ") << data.desc << nCrlf;   return false;
      }
    }

  notePad << title << _T(" error:  ") << status << nCrlf;   return false;
  }


