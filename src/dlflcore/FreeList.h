/*** ***/

/**
 * \file FreeList.h
 */

// Simple linked list for memory management. --- Fenghui
#ifndef _FREE_LIST_HH_
#define _FREE_LIST_HH_
namespace DLFL {
  // Pointer points to a free DLFLEdge in the memory.
  class NextOnFreeList {
  public:
    NextOnFreeList *next;
  };
}
#endif
