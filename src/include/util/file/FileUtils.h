void
CustomQuickchat::WriteBindingsToJson() {
    json bindingsJsonObj;

    for (const auto& binding : Bindings) {
        json singleBinding;

        singleBinding["chat"] = binding.chat;
        singleBinding["chatMode"] = static_cast<int>(binding.chatMode);
        singleBinding["bindingType"] = static_cast<int>(binding.bindingType);
        singleBinding["enabled"] = binding.enabled;

        singleBinding["buttons"] = {};
        for (const auto& button : binding.buttons) {
            singleBinding["buttons"].push_back(button);
        }

        bindingsJsonObj["bindings"].push_back(singleBinding);
    }

    Files::write_json(bindingsFilePath, bindingsJsonObj);
    LOG("Updated 'Bindings.json' :)");
}

void
CustomQuickchat::WriteVariationsToJson() {
    json variationsJsonObj;

    for (const auto& list : Variations) {
        json variationList;

        variationList["listName"] = list.listName;
        variationList["wordList"] = {};

        for (const auto& word : list.wordList) {
            variationList["wordList"].push_back(word);
        }

        variationsJsonObj["variations"].push_back(variationList);
    }

    Files::write_json(variationsFilePath, variationsJsonObj);
    LOG("Updated 'Variations.json' :)");
}

void
CustomQuickchat::GetFilePaths() {
    fs::path bmDataFolderFilePath = gameWrapper->GetDataFolder();

    customQuickchatFolder = bmDataFolderFilePath / "CustomQuickchat";
    bindingsFilePath = customQuickchatFolder / "Bindings.json";
    variationsFilePath = customQuickchatFolder / "Variations.json";

#if defined(USE_SPEECH_TO_TEXT)
    speechToTextJsonPath = customQuickchatFolder / "SpeechToText.json";
    speechToTextExePath = customQuickchatFolder / "SpeechToText" / "SpeechToText.exe";
    speechToTextErrorLogPath = customQuickchatFolder / "SpeechToText" / "ErrorLog.txt";
#endif

    lobbyInfoFolder = bmDataFolderFilePath / "Lobby Info";
    lobbyInfoChatsFilePath = lobbyInfoFolder / "Chats.json";
    lobbyInfoRanksFilePath = lobbyInfoFolder / "Ranks.json";
}
