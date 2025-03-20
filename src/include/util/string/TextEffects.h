#pragma once

#include <string>

std::string
ApplyTextEffect(const std::string& originalText, ETextEffect effect);

std::string
to_sarcasm(const std::string& ogText);

std::string
to_uwu(const std::string& ogText);