#pragma once

namespace TaskHandler {
  void SetTimeout(void function(), long millisecs);
  //void SetInterval(void function(), long millisecs);
  void ExecuteNextTask();

}
