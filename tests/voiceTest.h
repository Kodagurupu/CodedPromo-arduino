#ifndef VOISETEST_H
#define VOISETEST_H

#include "../voice.h"

class VoiceTest
{
public:
  VoiceTest() 
  {
    voice.init();  
  }
  void start()
  {
    voice.loopFunc();
  }

private:
  Voice voice;
};


#endif