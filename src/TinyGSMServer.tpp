#ifndef SRC_TINYGSMSERVER_H_
#define SRC_TINYGSMSERVER_H_

#include "TinyGsmCommon.h"

#define TINY_GSM_MODEM_HAS_TCP_SERVER

#include "TinyGsmFifo.h"

template <class modemType>
class TinyGsmTCPServer {
 public:

  /*
   * CRTP Helper
   */
 protected:
  inline const modemType& thisModem() const {
    return static_cast<const modemType&>(*this);
  }
  inline modemType& thisModem() {
    return static_cast<modemType&>(*this);
  }

  /*
   * Inner Server
   */
 public:
  class GsmServer {
    // Make all classes created from the modem template friends
    friend class TinyGsmTCPServer<modemType>;

   public:    
    bool startserver() {
      at->sendAT(GF("+CIPRXGET=1"));
      at->waitResponse(200);
      at->sendAT(GF("+SERVERSTART="), port, GF(", "), server_index);
      if (at->waitResponse(200) == 1) 
      { 
        server_running = true;
        return true; 
      } else {
        server_running = false;
        return false; 
      }
    }
    
    bool stop()  {
      at->sendAT(GF("+SERVERSTOP="), server_index);
      if (at->waitResponse(200) == 1) 
      { 
        server_running = false;
        return true; 
      } else {
        server_running = true;
        return false; 
      }
      return false;
    }

    bool running()  {
      return server_running;
    }

    modemType*  at;
    uint8_t     server_index;
    bool        server_running;
    uint16_t    port;
  };
};

#endif  // SRC_TINYGSMSERVER_H_
