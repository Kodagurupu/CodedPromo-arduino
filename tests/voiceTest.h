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
  int start()
  {
    return voice.loopFunc();
  }

private:
  Voice voice;
};


#endif
