// Memory Data


#pragma once


struct MemData {
void*  beg;
void*  end;
bool   allocated;
bool   deallocated;
int    lineNo;
String file;

  MemData() : beg(0), end(0), allocated(false), deallocated(false) { }
  };


class Memory {
int     n;
MemData memData[40];

public:

  Memory() : n(0) { }

  void alct(void* b, uint sz, Cchar* f, int ln);
  void dalct(void* b);

  void alct(HFONT h, TCchar* f, int ln);
  void dalct(HFONT h);

private:

  MemData* findEmpty(void* b, int& i);
  MemData* findExact(void* b);
  };


extern Memory memory;
