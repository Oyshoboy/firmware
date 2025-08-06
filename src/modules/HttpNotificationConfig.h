#pragma once

/**
 * HTTP Notification Module Configuration
 * 
 * This file contains all the configurable parameters for the HTTP notification module.
 * Modify these values to customize the module behavior.
 */

// Command prefix that triggers HTTP notifications
// Change this to use a different command (e.g., "/http", "/notify", etc.)
#define HTTP_NOTIFICATION_COMMAND_PREFIX "/tg"

// Default HTTP endpoint URL
// Replace with your actual endpoint URL
// For Telegram: "https://api.telegram.org/bot<YOUR_BOT_TOKEN>/sendMessage?chat_id=@your_channel_username&text="
// For Discord: "https://discord.com/api/webhooks/<WEBHOOK_ID>/<WEBHOOK_TOKEN>?content="
// For Slack: "https://hooks.slack.com/services/<WORKSPACE>/<CHANNEL>/<WEBHOOK_TOKEN>?text="
#define HTTP_NOTIFICATION_DEFAULT_URL "https://api.telegram.org/bot8471850605:AAFuKd0tXTs1o8xOO2Ki4w0Eu7xT9J2KLjg/sendMessage?chat_id=-1002741353767&text="

// Enable/disable HTTP notifications
#define HTTP_NOTIFICATION_ENABLED true

// HTTP request timeout in milliseconds
#define HTTP_NOTIFICATION_TIMEOUT_MS 10000

// Maximum URL length
#define HTTP_NOTIFICATION_MAX_URL_LENGTH 512

// Enable debug logging
#define HTTP_NOTIFICATION_DEBUG_LOGGING true 