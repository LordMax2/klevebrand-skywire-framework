#include <SoftwareSerial.h>

#define SKYWIRE_TX_PIN = 10
#define SKYWIRE_RX_PIN = 11

class Skywire {
    public: 
      void start();
      void send();
      SoftwareSerial skywire;
    private:
      void stop();
};