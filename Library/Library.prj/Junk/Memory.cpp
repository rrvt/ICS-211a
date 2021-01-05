// Memory Data


#include "stdafx.h"
#include "Memory.h"


void Memory::alct(void* b, uint sz, Cchar* f, int ln) {
int      i;
MemData*  md   = findEmpty(b, i);        if (!b) return;
int       p    = (int) b + sz - 1;
void*     e    = (void*) p;
ToUniCode fl(f);
String    file = fl();
int       pos;

  if (n >= noElements(memData)) {
int x = 1;  return;
  }

  if (md && !md->deallocated) {
    int x = 1;
    }

  if (!md) md = &memData[n++];

  pos = file.findLastOf(_T('\\'));  if (pos >= 0) file = file.substr(pos);

  md->beg = b; md->end = e; md->file = file;   md->lineNo = ln;

  md->allocated = true;  md->deallocated = false;
  }


void Memory::dalct(void* b) {
MemData* md = findExact(b);      if (!b) return;
  if (md) {md->deallocated = true; return;}

  int x = 1;
  }


void Memory::alct(HFONT h, TCchar* f, int ln) {
void*    b  = h;              if (!h) return;
int      i;
MemData* md = findEmpty(b, i);

  if (n >= noElements(memData)) {
int x = 1;  return;
  }

  if (md && !md->deallocated) {
    int x = 1;
    }

if (i == 23) {
int x = 1;
}
if (i == 24) {
int x = 1;
}

  if (!md) md = &memData[n++];

  md->beg = b; md->end = 0; md->file = f; md->lineNo = ln;

  md->allocated = true;  md->deallocated = false;
  }


void Memory::dalct(HFONT h) {
void*    b  = h;               if (!h) return;
MemData* md = findExact(b);

  if (md) {md->deallocated = true; return;}

  md = &memData[n++];

  md->beg = b; md->end = (void*) -1; md->allocated = false; md->deallocated = true;
  }



MemData* Memory::findEmpty(void* b, int& i) {
MemData* md;

  if (!b) return 0;

  for (i = 0; i < n; i++) {
    md = &memData[i];

    if ((md->deallocated && md->allocated) || md->beg == b) return md;;
    }

  return 0;
  }


MemData* Memory::findExact(void* b) {
int      i;
MemData* md;

  for (i = 0; i < n; i++) {md = &memData[i];  if (md->beg == b) return md;}

  return 0;
  }

