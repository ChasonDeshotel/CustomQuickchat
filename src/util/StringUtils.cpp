auto
StringUtils::process_keywords_in_chat_str(const Binding& binding) -> std::string {
    std::string result = binding.chat;

    for (int i = 0; i < MAX_KEYWORD_DEPTH; i++) {
        auto keyword_strings_to_replace = binding.GetMatchedSubstrings(result, keywordRegexPattern);

        if (keyword_strings_to_replace.empty()) {
            if (i > 1) {
                LOG("Resolved nested variation(s) using {} substitution passes", i);
            }

            break;
        }

        for (const auto& keyword_str_to_replace : keyword_strings_to_replace) {
            const std::string regexPatternStr = "\\[\\[" + keyword_str_to_replace + "\\]\\]";
            std::regex keyword_regex_pattern(regexPatternStr);

            // if a special keyword was found (not a variation list name) .... which, at this point, would be just one of the lastChat
            // keywords
            auto it = keywordsMap.find(keyword_str_to_replace);
            if (it != keywordsMap.end()) {
                std::string lastChat;

                switch (it->second) {
                    case EKeyword::LastChat:
                    case EKeyword::LastChatUwu:
                    case EKeyword::LastChatSarcasm:
                        lastChat = get_last_chat();
                        if (lastChat.empty())
                            return "";
                        lastChat = ApplyTextEffect(lastChat, binding.textEffect);
                        result = std::regex_replace(result, keyword_regex_pattern, lastChat);
                        break;
                    default: break; // this should never get executed bc keyword should be a lastChat atp, but who knows
                }
            }
            // if something else was found, like a word variation list name
            else {
                result = std::regex_replace(result, keyword_regex_pattern, Variation(keyword_str_to_replace));
            }
        }
    }

    return result;
}
