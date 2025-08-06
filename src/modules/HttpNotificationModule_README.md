# HTTP Notification Module for Meshtastic

A universal HTTP notification module that sends HTTP requests when specific message patterns are detected in the Meshtastic mesh network.

## Features

- **Universal Design**: Works with any HTTP endpoint (Telegram, Discord, Slack, custom webhooks, etc.)
- **Isolated Module**: Doesn't touch the original firmware codebase
- **Configurable**: Easy to customize via configuration file
- **URL Encoding**: Automatically encodes messages for safe HTTP transmission
- **Error Handling**: Robust error handling with logging
- **WiFi Aware**: Only sends requests when WiFi is connected

## How It Works

1. **Message Detection**: Monitors all text messages in the mesh network
2. **Command Recognition**: Looks for messages starting with `/tg` (configurable)
3. **Message Processing**: Extracts the message content after the command prefix
4. **HTTP Request**: Sends a GET request to the configured endpoint with the encoded message
5. **Response Handling**: Logs the response for debugging

## Configuration

Edit `src/modules/HttpNotificationConfig.h` to customize the module:

```cpp
// Command prefix that triggers HTTP notifications
#define HTTP_NOTIFICATION_COMMAND_PREFIX "/tg"

// Your HTTP endpoint URL
#define HTTP_NOTIFICATION_DEFAULT_URL "https://api.telegram.org/bot<YOUR_BOT_TOKEN>/sendMessage?chat_id=@your_channel_username&text="

// Enable/disable the module
#define HTTP_NOTIFICATION_ENABLED true

// HTTP timeout in milliseconds
#define HTTP_NOTIFICATION_TIMEOUT_MS 10000

// Maximum URL length
#define HTTP_NOTIFICATION_MAX_URL_LENGTH 512

// Enable debug logging
#define HTTP_NOTIFICATION_DEBUG_LOGGING true
```

## Usage Examples

### Telegram Bot
```
/tg Hello from Meshtastic!
```
Sends: `https://api.telegram.org/bot<BOT_TOKEN>/sendMessage?chat_id=@channel&text=Hello%20from%20Meshtastic!`

### Discord Webhook
```
/tg Alert: Node 0x1234 is offline
```
Sends: `https://discord.com/api/webhooks/<WEBHOOK_ID>/<TOKEN>?content=Alert%3A%20Node%200x1234%20is%20offline`

### Custom Webhook
```
/tg {"event": "message", "data": "Test message"}
```
Sends: `https://your-webhook.com/endpoint?text=%7B%22event%22%3A%20%22message%22%2C%20%22data%22%3A%20%22Test%20message%22%7D`

## Setup Instructions

1. **Configure the endpoint URL** in `HttpNotificationConfig.h`
2. **Build the firmware** with the module included
3. **Flash to your device**
4. **Send messages** starting with `/tg` followed by your message

## Supported Platforms

- ESP32 (all variants)
- ESP32-S3
- ESP32-C3
- ESP32-C6
- Any platform with WiFi support

## Customization

### Change Command Prefix
```cpp
#define HTTP_NOTIFICATION_COMMAND_PREFIX "/http"
```

### Use Different Endpoint
```cpp
// Discord
#define HTTP_NOTIFICATION_DEFAULT_URL "https://discord.com/api/webhooks/<WEBHOOK_ID>/<WEBHOOK_TOKEN>?content="

// Slack
#define HTTP_NOTIFICATION_DEFAULT_URL "https://hooks.slack.com/services/<WORKSPACE>/<CHANNEL>/<WEBHOOK_TOKEN>?text="

// Custom webhook
#define HTTP_NOTIFICATION_DEFAULT_URL "https://your-api.com/webhook?message="
```

### Disable Module
```cpp
#define HTTP_NOTIFICATION_ENABLED false
```

## Troubleshooting

### WiFi Not Connected
The module will log: `HTTP Notification: WiFi not connected, skipping HTTP request`

### HTTP Request Failed
Check the response code in the logs. Common issues:
- Invalid URL
- Network connectivity
- Endpoint not responding
- Authentication issues

### URL Too Long
Increase `HTTP_NOTIFICATION_MAX_URL_LENGTH` if you're sending long messages.

## Security Considerations

- **Bot Tokens**: Keep your bot tokens secure
- **Webhook URLs**: Don't share webhook URLs publicly
- **Message Content**: Be aware that messages are sent over HTTP (not encrypted)
- **Rate Limiting**: Some services have rate limits

## Building

The module is automatically included when you build the firmware. No additional build flags are required.

## License

This module follows the same license as the Meshtastic firmware. 