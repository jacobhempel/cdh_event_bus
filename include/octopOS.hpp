#ifndef OCTOPOS_H
#define OCTOPOS_H

#include <vector>
#include <iostream>

using std::vector;
using std::cout;

class SPACEHAUC_thread {
public:
   SPACEHAUC_thread() {}
   virtual ~SPACEHAUC_thread() {}
   bool StartInternalThread() {
     return (pthread_create(&_thread, NULL, InternalThreadEntryFunc,this) == 0);
   }

   /** Will not return until the internal thread has exited. */
   void WaitForInternalThreadToExit() {
      (void) pthread_join(_thread, NULL);
   }

protected:
   /** Implement this method in your subclass with the code you want your thread to run. */
   virtual void InternalThreadEntry() = 0;

private:
   static void * InternalThreadEntryFunc(void * This) {((SPACEHAUC_thread *)This)->InternalThreadEntry(); return NULL;}

   pthread_t _thread;
};

#endif  // OCTOPOS_H
