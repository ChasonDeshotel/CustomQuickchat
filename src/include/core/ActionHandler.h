#pragma once

class ActionHandler {
  public:
    void exitToMainMenu(std::vector<std::string> args);
    void forfeit(std::vector<std::string> args);

    std::string process_keywords_in_chat_str(const Binding& binding);

    std::string get_last_chat();
    std::string get_last_chatter_rank_str(EKeyword keyword);
};