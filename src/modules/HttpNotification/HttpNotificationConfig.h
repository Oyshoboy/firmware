#pragma once
#include "HttpNotificationSecrets.h"

#define HTTP_NOTIFICATION_COMMAND_PREFIX "/tg"

#define HTTP_NOTIFICATION_DEFAULT_URL "https://api.telegram.org/bot" HTTP_NOTIFICATION_BOT_TOKEN \
    "/sendMessage?chat_id=" HTTP_NOTIFICATION_CHAT_ID "&text="

#define HTTP_NOTIFICATION_ENABLED true

#define HTTP_NOTIFICATION_TIMEOUT_MS 10000

#define HTTP_NOTIFICATION_MAX_URL_LENGTH 512

#define HTTP_NOTIFICATION_DEBUG_LOGGING true