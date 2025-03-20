#pragma once
#include <string>

class TextEffects {
  public:
    std::string ApplyTextEffect(const std::string& originalText, ETextEffect effect);

    std::string to_sarcasm(const std::string& ogText);

    std::string to_uwu(const std::string& ogText);
};