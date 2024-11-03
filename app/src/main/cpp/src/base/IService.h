#pragma once

class IService {
  public:
    virtual void run() = 0;
    virtual void shutdown() = 0;
  private:
    bool shutdownFlag = false;
};