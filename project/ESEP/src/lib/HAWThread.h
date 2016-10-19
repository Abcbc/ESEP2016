/***************************************************************************
*                                                                         
*    Hochschule für Angewandte Wissenschaften Hamburg              
*                                                                          
*    WP
*    Effiziente Datenstrukturen in C++ für verteilte Echtzeitprogrammierung
*
*	 Prof. Dr. Stephan Pareigis  
*
*    Thread Class                                                                         
****************************************************************************/

#ifndef _HAW_THREAD_H_
#define _HAW_THREAD_H_

#include <iostream>
#include <pthread.h>
#include <sys/neutrino.h>
using namespace std;

/**
 * HAWThread class.
 * Encapsulates the most important features of a thread.
 * This serves as a basis for further development.
 * For example, priorities could be passed as constructor 
 * argument.
 */
namespace thread 
{
class HAWThread
{
   public:
	   /**
	   * Constructor. Initializes members
	   */
      HAWThread();
	  /**
	  * Destructor. Calls ThreadDestroy. This should
	  * shutdown the thread more carefully. 
	  * @warning needs some work!
	  */
      virtual ~HAWThread();
      /**
      *  Starts the Thread.
      *  @warning start must be called always from the same context as stop().
      *  @warning If Thread is already running, start() is a NOP.
	  *  @arg argument is stored locally as a member.
      */
	  virtual void start(void * arg);
	  /**
	  * Sets the internal flag LOCAL_EXIT to true.
	  */ 
      void stop();
	  /**
	  * Calls join on the thread.
	  * @warning must be called from the same context as start.
	  */
	  void join() const;
	  /**
	   * This function holds (suspends) the thread. It makes a ThreadCtrl call. 
	   * It shall be used if the thread is not being used for a while but 
	   * may be used later
	   */
	  void hold();
	  /** 
	   * This function continues (resumes) the thread. It makes a ThreadCtrl call. 
	   */
	  void cont();
   protected:
	   /** This is called when the thread is started.
	   * It calls execute an shutdown which are 
	   * the user functions.
	   */
      void run(void * arg);
      static void* entryPoint(void*);
	  /**
	  * to be implemented in the derived class.
	  * The application programmer has to write his own loop.
	  * He can check bool isStopped() to see if the thread
	  * should exit the loop.
	  */
      virtual void execute(void*)=0;
	  /** 
	  * this function must be implemented 
	  * in the derived class.
	  * The function is called once after
	  * the thread has been stopped.
	  */
      virtual void shutdown()=0;
	  /**
	  * used internally to pass the argument.
	  */
      void * Arg() const {return Arg_;}
	  /** 
	  * used internally to set the arguement.
	  */
      void Arg(void* a){Arg_ = a;}
	  /**
	  * returns the stop-status of the thread.
	  * This function should be checked by 
	  * the user function execute regularly.
	  */
	  bool isStopped() const { return (GLOBAL_EXIT || LOCAL_EXIT);}
	  /** sets the GLOBAL_EXIT flag to true.*/		
	  void shutdownAll() { GLOBAL_EXIT = true; }
   private:
	  pthread_t tid_;/**< thread id to be passed to pthread_create*/
      volatile bool bRunning_;/**< flag to check if thread is running*/
      void * Arg_;/**< argument stored locally. To be passed to user function.*/
      static volatile bool GLOBAL_EXIT;/**< if true, then all threads will close, if function isStopped is checked.*/
      bool LOCAL_EXIT;/**< stops the local thread, if isStopped is checked.*/
};
};

#endif
