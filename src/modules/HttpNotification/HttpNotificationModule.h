#pragma once
#include "Observer.h"
#include "SinglePortModule.h"
#include "HttpNotificationConfig.h"
#include <HTTPClient.h>

/**
 * HTTP notification module for meshtastic - sends HTTP requests when specific message patterns are detected
 */
class HttpNotificationModule : public SinglePortModule, public Observable<const meshtastic_MeshPacket *>
{
  public:
    HttpNotificationModule() : SinglePortModule("httpnotify", meshtastic_PortNum_TEXT_MESSAGE_APP) {}

  protected:
    virtual ProcessMessage handleReceived(const meshtastic_MeshPacket &mp) override;
    virtual bool wantPacket(const meshtastic_MeshPacket *p) override;

  private:
    void sendHttpNotification(const char* message);
    void processHttpCommand(const char* payload, size_t payloadSize);
    bool isHttpCommand(const char* payload, size_t payloadSize);
    char* urlEncode(const char* str);
    
    HTTPClient httpClient;
    static const char* HTTP_COMMAND_PREFIX;
    static const char* DEFAULT_HTTP_URL;
};

extern HttpNotificationModule *httpNotificationModule; 