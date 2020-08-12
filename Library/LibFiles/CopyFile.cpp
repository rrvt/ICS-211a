//  Copy and/or Move (Rename) File
// rrvt 6/5/16


#include "stdafx.h"
#include "CopyFile.h"
#include "Expandable.h"
#include "filename.h"
#include "filesrch.h"

#ifndef Win2K
#include <filesystem>
using namespace std::experimental::filesystem;
#endif


typedef Expandable<String, 16> Names;


static bool bkupFileName(TCchar* filePath, String& bkupName, Names& names);
static void removeExcess(Names& names, int noBackups);


#ifdef Win2K

bool copyFile(String& source, String& dst) {
CFile  srcFile;
int    nRead;
Byte   buf[4096];
HANDLE hFile = CreateFile(dst, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS,
                                                                            FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile == INVALID_HANDLE_VALUE) return false;

CFile dstFile(hFile);                                   // Attach a CFile object to the handle we have.

  if (!srcFile.Open(source, CFile::modeRead | CFile::shareDenyWrite)) {dstFile.Close(); return false;}

  while (nRead = srcFile.Read(buf, noElements(buf))) dstFile.Write(buf, nRead);

  srcFile.Close(); dstFile.Close(); return true;
  }

#else


bool copyFile(TCchar* source, String& dst) {

  return copy_file(source, dst,  copy_options::overwrite_existing);
  }

#endif



void backupFile(TCchar* filePath, int noBackups) {
Names  names;
String bkupName;

  if (bkupFileName(filePath, bkupName, names))
                                        {moveFile(filePath, bkupName);   removeExcess(names, noBackups);}
  }


void backupCopy(TCchar* filePath, int noBackups) {
String bkupName;
Names  names;

  if (bkupFileName(filePath, bkupName, names)) {
                                        copyFile(filePath, bkupName);  removeExcess(names, noBackups);}
  }


bool isFilePresent(TCchar*  path) {
WIN32_FIND_DATA findData;
HANDLE          h;

  if ((h = FindFirstFile(path, &findData)) ==  INVALID_HANDLE_VALUE) return false;

  FindClose(h);   return true;
  }



bool bkupFileName(TCchar* filePath, String& bkupName, Names& names) {
String                 name;
String                 path;
FileSrch               srch;
int                    i;
String                 time;

  bkupName.clear();

  if (!isFilePresent(filePath)) return false;

  name = removePath(filePath) + _T(".*");
  path = getPath(filePath);

  if (srch.findFiles(path, name)) {
    for (i = 0; srch.getName(bkupName); i++) names[i] = bkupName;
    }

  CTime t = CTime::GetCurrentTime();   time = t.Format(_T("%C%m%d%H%M%S"));

  bkupName = filePath;  bkupName += _T('.');  bkupName += time;

  return true;
  }


void removeExcess(Names& names, int noBackups) {
int n = names.end() - noBackups + 1;
int i;

  for (i = 0; i < n; i++) _tremove(names[i]);
  }



#if 0
int backupFile(String& path, int lastBackup, int nBackups) {
String backupFile;
int    i;
int    n = 100 - nBackups;
int    j;
int    k;

  if (!isFilePresent(path)) return -1;

  if (lastBackup <= 0) {
    for (i = 0; i < 100; i++) {
      backupFile.format(_T("%s.%02i"), path.str(), i);

      if (isFilePresent(backupFile)) {lastBackup = i; break;}
      }
    }

  for (i = lastBackup; i < n; i++)
                {backupFile.format(_T("%s.%02i"), path.str(), i);   if (!isFilePresent(backupFile)) break;}

  moveFile(path, backupFile);

  if (nBackups) {

    for (j = 0, k = i - nBackups; j < n; j++, k--) {

      if (k < 0) k += 100;    if (k == i) break;

      backupFile.format(_T("%s.%02i"), path.str(), k);

      if (!isFilePresent(backupFile)) break;
#ifndef Win2K
      std::experimental::filesystem::remove(backupFile);
#endif
      }
    }

  return i;
  }
#endif



#if 0
String                 name;
String                 path;
FileSrch               srch;
int                    i;
int                    n;
String                 time;

  if (!isFilePresent(filePath)) return;

  name = removePath(filePath) + _T(".*");
  path = getPath(filePath);

  if (srch.findFiles(path, name)) {
    for (i = 0; srch.getName(bkupName); i++) names[i] = bkupName;
    }

  CTime t = CTime::GetCurrentTime();   time = t.Format(_T("%C%m%d%H%M%S"));

  bkupName = filePath + _T('.') + time;
#endif
#if 0
  n = names.end() - noBackups + 1;

  for (i = 0; i < n; i++) _tremove(names[i]);
#endif
#if 0
String                 name;
String                 path;
FileSrch               srch;
int                    i;
int                    n;
String                 time;

  if (!isFilePresent(filePath)) return;

  name = removePath(filePath) + _T(".*");
  path = getPath(filePath);

  if (srch.findFiles(path, name)) {
    for (i = 0; srch.getName(bkupName); i++) names[i] = bkupName;
    }

  CTime t = CTime::GetCurrentTime();   time = t.Format(_T("%C%m%d%H%M%S"));

  bkupName = filePath + _T('.') + time;
#endif


#if 0
  n = names.end() - noBackups + 1;

  for (i = 0; i < n; i++) _tremove(names[i]);
#endif

