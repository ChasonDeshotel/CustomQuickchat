#pragma once

class NotificationManager {
  public:
    void SpawnNotification(const std::string& title, const std::string& content, int duration, bool log = false);
    UClass* notificationClass;

    void NotifyAndLog(const std::string& title, const std::string& message, int duration = 3);
};