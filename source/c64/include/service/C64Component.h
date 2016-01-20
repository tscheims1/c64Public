#ifndef C64COMPONENT_H
#define C64COMPONENT_H
#include "MemoryManager.h"

struct _6510_cpu {
  char pcl[9];
  char pch[9];
  char regx[9];
  char regy[9];
  char rega[9];
  char flags[9];
  char sp[9];

  char abrl[9];
  char abrh[9];
  char dbr[9];
  char ir[9];
  char rw;
  char interrupt;

  int cycles;
};


namespace Service
{


class MemoryManager;

/**
 * Just an abstract interface for proper polymorphismus
 */
class C64Component
{
    public:
        C64Component();
        virtual ~C64Component();
        virtual void readMemory(MemoryManager* memManager) =0;//Pure Abstract Class (=Interface)
        virtual bool isAsync() =0;

    protected:

    private:
};
};
#endif // C64COMPONENT_H
