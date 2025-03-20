void
NotificationManager::SpawnNotification(const std::string& title, const std::string& content, int duration, bool log) {
    UNotificationManager_TA* notificationManager = Instances.GetInstanceOf<UNotificationManager_TA>();
    if (!notificationManager)
        return;

    if (!notificationClass) {
        notificationClass = UGenericNotification_TA::StaticClass();
    }

    UNotification_TA* notification = notificationManager->PopUpOnlyNotification(notificationClass);
    if (!notification)
        return;

    FString titleFStr = Instances.NewFString(title);
    FString contentFStr = Instances.NewFString(content);

    notification->SetTitle(titleFStr);
    notification->SetBody(contentFStr);
    notification->PopUpDuration = duration;

    if (log) {
        LOG("[{}] {}", title.c_str(), content.c_str());
    }
}
