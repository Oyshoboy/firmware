#include "HttpNotificationModule.h"
#include "MeshService.h"
#include "NodeDB.h"
#include "configuration.h"
#include <WiFi.h>
#include <cstring>

HttpNotificationModule *httpNotificationModule;

const char* HttpNotificationModule::HTTP_COMMAND_PREFIX = HTTP_NOTIFICATION_COMMAND_PREFIX;
const char* HttpNotificationModule::DEFAULT_HTTP_URL = HTTP_NOTIFICATION_DEFAULT_URL;

ProcessMessage HttpNotificationModule::handleReceived(const meshtastic_MeshPacket &mp)
{
    if (!HTTP_NOTIFICATION_ENABLED) {
        return ProcessMessage::CONTINUE;
    }
    
    auto &p = mp.decoded;
    
    if (p.payload.size > 0) {
        char payload[p.payload.size + 1];
        memcpy(payload, p.payload.bytes, p.payload.size);
        payload[p.payload.size] = '\0';
        
        // Get sender short ID and format message
        NodeNum senderNodeNum = mp.from;
        
        if (senderNodeNum != nodeDB->getNodeNum()) {
            sendHttpNotification(payload);
        }
    }
    
    return ProcessMessage::CONTINUE;
}

bool HttpNotificationModule::wantPacket(const meshtastic_MeshPacket *p)
{
    return MeshService::isTextPayload(p);
}

bool HttpNotificationModule::isHttpCommand(const char* payload, size_t payloadSize)
{
    if (payloadSize < strlen(HTTP_COMMAND_PREFIX)) {
        return false;
    }
    
    return strncmp(payload, HTTP_COMMAND_PREFIX, strlen(HTTP_COMMAND_PREFIX)) == 0;
}

void HttpNotificationModule::processHttpCommand(const char* payload, size_t payloadSize)
{
    const char* messageStart = payload + strlen(HTTP_COMMAND_PREFIX);
    
    while (*messageStart == ' ' && messageStart < payload + payloadSize) {
        messageStart++;
    }
    
    if (messageStart >= payload + payloadSize) {
        if (HTTP_NOTIFICATION_DEBUG_LOGGING) {
            LOG_INFO("HTTP Notification: Empty message, skipping\n");
        }
        return;
    }
    
    size_t messageLength = payloadSize - (messageStart - payload);
    char message[messageLength + 1];
    strncpy(message, messageStart, messageLength);
    message[messageLength] = '\0';
    
    if (HTTP_NOTIFICATION_DEBUG_LOGGING) {
        LOG_INFO("HTTP Notification: Processing command: %s\n", message);
    }
    sendHttpNotification(message);
}

void HttpNotificationModule::sendHttpNotification(const char* message)
{
    if (WiFi.status() != WL_CONNECTED) {
        if (HTTP_NOTIFICATION_DEBUG_LOGGING) {
            LOG_INFO("HTTP Notification: WiFi not connected, skipping HTTP request\n");
        }
        return;
    }
    
    char meshId[16];
    snprintf(meshId, sizeof(meshId), "%04x", nodeDB->getNodeNum() & 0xFFFF);
    
    char fullMessage[HTTP_NOTIFICATION_MAX_URL_LENGTH];
    snprintf(fullMessage, sizeof(fullMessage), "%s: %s", meshId, message);
    
    char* encodedMessage = urlEncode(fullMessage);
    if (!encodedMessage) {
        LOG_ERROR("HTTP Notification: Failed to encode message\n");
        return;
    }
    
    char url[HTTP_NOTIFICATION_MAX_URL_LENGTH];
    if (snprintf(url, sizeof(url), "%s%s", DEFAULT_HTTP_URL, encodedMessage) >= sizeof(url)) {
        LOG_ERROR("HTTP Notification: URL too long\n");
        free(encodedMessage);
        return;
    }
    
    if (HTTP_NOTIFICATION_DEBUG_LOGGING) {
        LOG_INFO("HTTP Notification: Sending request to: %s\n", url);
    }
    
    httpClient.begin(url);
    httpClient.setTimeout(HTTP_NOTIFICATION_TIMEOUT_MS);
    
    int httpResponseCode = httpClient.GET();
    
    if (httpResponseCode > 0) {
        String response = httpClient.getString();
        if (HTTP_NOTIFICATION_DEBUG_LOGGING) {
            LOG_INFO("HTTP Notification: Response code %d, response: %s\n", httpResponseCode, response.c_str());
        }
    } else {
        LOG_ERROR("HTTP Notification: Error on HTTP request, code: %d\n", httpResponseCode);
    }
    
    httpClient.end();
    free(encodedMessage);
}

char* HttpNotificationModule::urlEncode(const char* str)
{
    if (!str) return nullptr;
    
    size_t len = strlen(str);
    char* encoded = (char*)malloc(len * 3 + 1);
    if (!encoded) return nullptr;
    
    char* ptr = encoded;
    
    for (size_t i = 0; i < len; i++) {
        char c = str[i];
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || 
            c == '-' || c == '_' || c == '.' || c == '~') {
            *ptr++ = c;
        } else if (c == ' ') {
            *ptr++ = '%';
            *ptr++ = '2';
            *ptr++ = '0';
        } else {
            sprintf(ptr, "%%%02X", (unsigned char)c);
            ptr += 3;
        }
    }
    
    *ptr = '\0';
    return encoded;
} 