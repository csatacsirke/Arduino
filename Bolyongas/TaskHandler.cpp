#include "TaskHandler.h"
#include <Arduino.h>
namespace TaskHandler {

  
  struct Task {
    //void function(void);
    void(*function)(void);
    long startTime;

    Task() : function(0), startTime(0) {}
    Task(void function(void), long startTime) : function(function), startTime(startTime) {}

    inline void Execute() { function(); }
  };

  
  
  template<int max_size>
  class PriorityQueue {
    Task tasks[max_size];  
    int size;
  public:
    PriorityQueue() :size(0) {}

    void Push(Task task) {
      int current = size;
      tasks[current] = task;
      ++size;
      while(current != 0 && tasks[current].startTime < tasks[ParentIndex(current)].startTime ) {
        Swap(current, ParentIndex(current));
        current = ParentIndex(current);
      }
    }
    
    Task Pop() {
      Task task = tasks[0]; 
      --size;
      Swap(0, size);

      int current = 0;
      while( true ) {
        if( RightChildIndex(current) < size && LeftChildIndex(current) < size ) {
          if( StartTime(current) < RightChild(current) &&  StartTime(current) < LeftChild(current) ) {
            // kisebb mindenkinél a felső
            break;
          } else if( LeftChild(current) < RightChild(current) ) {
            Swap(LeftChildIndex(current), current);
            current = LeftChildIndex(current);
          } else {
            Swap(RightChildIndex(current), current);
            current = RightChildIndex(current);
          }
        } else if(RightChildIndex(current) < size ) {
          if( StartTime(current) < RightChild(current) ) {
            break;
          } else {
            Swap(RightChildIndex(current), current);
            current = RightChildIndex(current);
          }
        } else if(LeftChildIndex(current) < size ){
          if( StartTime(current) < LeftChild(current) ) {
            break;
          } else {
            Swap(LeftChildIndex(current), current);
            current = LeftChildIndex(current);
          }
        } else {
          break;
        }
      }
      return task;
    }
    
    Task& Top() { return tasks[0]; };
    bool IsEmpty() {return (size == 0); }
    static int ParentIndex(int i) { return (i/2 - 1); }
    static int RightChildIndex(int i) { return ( (i+1)*2 ); }
    static int LeftChildIndex(int i) { return ( (i+1)*2-1 ); }
    long LeftChild(int i) { return tasks[LeftChildIndex(i)].startTime;}
    long RightChild(int i) { return tasks[RightChildIndex(i)].startTime;}
    long StartTime(int i) { return tasks[i].startTime; }
    void Swap(int i, int j) { Task temp = tasks[i]; tasks[i] = tasks[j]; tasks[j] = temp; }

  };
  
  PriorityQueue<20> queue;
  
  void SetTimeout(void function(void), long millisecs) {
    queue.Push(Task(function, millis() + millisecs));
  }

  void ExecuteNextTask() {
    if( ! queue.IsEmpty() ) {
      Task& task = queue.Top();
      if( task.startTime < millis() ) {
        task.Execute();
        queue.Pop();
      }
    }
  }
  
  
  

}
