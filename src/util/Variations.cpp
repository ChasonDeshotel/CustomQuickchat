auto
Variations::Variation(const std::string& listName) -> std::string {
    for (int i = 0; i < Variations.size(); i++) {
        VariationList& list = Variations[i];

        if (list.listName == listName) {
            if (list.wordList.size() < 3) {
                LOG("** '{}' word variation list has less than 3 items... and cannot be used **", listName);
                return listName;
            }

            std::string variation = list.shuffledWordList[list.nextUsableIndex];

            if (list.nextUsableIndex != (list.shuffledWordList.size() - 1)) {
                list.nextUsableIndex++;
                return variation;
            } else {
                ReshuffleWordList(i);
                return variation;
            }
        }
    }
    return listName;
}

void
Variations::UpdateDataFromVariationStr() {
    for (auto& variation : Variations) // <--- not const bc variation instances should be modified
    {
        // update word list based on parsed string
        std::vector<std::string> parsedVariations = Format::SplitStrByNewline(variation.unparsedString);
        variation.wordList = parsedVariations;

        // reset & create new shuffled word list
        variation.nextUsableIndex = 0;
        variation.shuffledWordList = ShuffleWordList(variation.wordList);
    }
}

auto
Variations::ShuffleWordList(const std::vector<std::string>& ogList) -> std::vector<std::string> {
    std::vector<std::string> shuffledList = ogList;

    if (ogList.size() >= 3) {
        std::random_device rd;  // Initialize random number generator
        std::mt19937 rng(rd()); // Mersenne Twister 19937 generator
        std::shuffle(shuffledList.begin(), shuffledList.end(), rng);
    }

    return shuffledList;
}

void
Variations::ReshuffleWordList(int idx) {
    auto& variationList = Variations[idx];
    std::vector<std::string> prevShuffled = variationList.shuffledWordList;

    // skip all the non-repetition BS if the list has less than 4 variations... and just shuffle it
    if (prevShuffled.size() < 4) {
        prevShuffled = ShuffleWordList(prevShuffled);
        variationList.shuffledWordList = prevShuffled;
        variationList.nextUsableIndex = 0;
        return;
    }

    // save last two words from previous shuffled list
    std::vector<std::string> last2Words;
    last2Words.push_back(prevShuffled[prevShuffled.size() - 1]);
    last2Words.push_back(prevShuffled[prevShuffled.size() - 2]);

    // create new shuffled list
    std::vector<std::string> shuffledBih = ShuffleWordList(variationList.wordList);

    std::string newShuffled1st = "";
    std::string newShuffled2nd = "";

    // find 1st different variation
    for (int i = 0; i < shuffledBih.size(); i++) {
        auto word = shuffledBih[i];

        auto it = std::find(last2Words.begin(), last2Words.end(), word);
        if (it == last2Words.end() && newShuffled1st == "") {
            newShuffled1st = word;
            shuffledBih.erase(shuffledBih.begin() + i);
            break;
        }
    }

    // find 2nd different variation
    for (int i = 0; i < shuffledBih.size(); i++) {
        auto word = shuffledBih[i];

        auto it = std::find(last2Words.begin(), last2Words.end(), word);
        if (it == last2Words.end() && newShuffled2nd == "") {
            newShuffled2nd = word;
            shuffledBih.erase(shuffledBih.begin() + i);
            break;
        }
    }

    // insert selected words (that are diff than prev last two) at beginning of new shuffled vector
    shuffledBih.insert(shuffledBih.begin(), newShuffled1st);
    shuffledBih.insert(shuffledBih.begin() + 1, newShuffled2nd);

    // update actual variation list info
    variationList.shuffledWordList = shuffledBih;
    variationList.nextUsableIndex = 0;
}
void
Variations::DeleteVariationList(int idx) {
    if (Variations.empty())
        return;

    // erase variation list at given index
    Variations.erase(Variations.begin() + idx);

    // reset selected variation list index
    selectedVariationIndex = Variations.empty() ? 0 : Variations.size() - 1;

    // update JSON
    WriteVariationsToJson();
}

// fixme probably move these to a file module
void
CustomQuickchat::CheckJsonFiles() {
    // create 'CustomQuickchat' folder if it doesn't exist
    if (!fs::exists(customQuickchatFolder)) {
        fs::create_directory(customQuickchatFolder);
        LOG("'CustomQuickchat' folder didn't exist... so I created it.");
    }

    // create JSON files if they don't exist
    if (!fs::exists(bindingsFilePath)) {
        std::ofstream NewFile(bindingsFilePath);

        NewFile << "{ \"bindings\": [] }";
        NewFile.close();
        LOG("'Bindings.json' didn't exist... so I created it.");
    }
    if (!fs::exists(variationsFilePath)) {
        std::ofstream NewFile(variationsFilePath);
        NewFile << "{ \"variations\": [] }";
        NewFile.close();
        LOG("'Variations.json' didn't exist... so I created it.");
    }
}

void
CustomQuickchat::ReadDataFromJson() {
    json bindings_json_data = Files::get_json(bindingsFilePath);

    if (!bindings_json_data.empty() && bindings_json_data.contains("bindings")) {
        auto bindingsList = bindings_json_data.at("bindings");

        if (bindingsList.size() > 0) {
            for (int i = 0; i < bindingsList.size(); i++) {
                // read data from each binding obj and update Bindings vector
                auto bindingObj = bindingsList[i];

                Binding binding;
                binding.chat = bindingObj.value("chat", "im gay");
                binding.chatMode = bindingObj.value("chatMode", EChatChannel::EChatChannel_Match);
                binding.bindingType = bindingObj.value("bindingType", EBindingType::Combination);

                // use value for "enabled" key if it exists, otherwise default to true
                // (to make backwards compatible with old json files that don't contain an "enabled" key)
                binding.enabled = bindingObj.value("enabled", true);

                if (bindingObj.contains("buttons") && bindingObj.at("buttons").is_array()) {
                    binding.buttons = bindingObj.value("buttons", std::vector<std::string>{});
                } else {
                    LOG("[ERROR] Missing or invalid \"buttons\" array in JSON");
                }

                // lastly, update binding's keyWord and textEffect values (which depend on the chat value above)
                binding.UpdateKeywordAndTextEffect(keywordRegexPattern);

                Bindings.push_back(binding);
            }
        }
    }

    // ... same thing for variations
    json variations_json_data = Files::get_json(variationsFilePath);

    if (!variations_json_data.empty() && variations_json_data.contains("variations")) {
        auto variations_list = variations_json_data.at("variations");

        if (variations_list.size() > 0) {
            for (int i = 0; i < variations_list.size(); i++) {
                // read data from each variation list obj and update Variations vector
                auto variationListObj = variations_list[i];

                VariationList variationList;
                variationList.listName = variationListObj["listName"];

                for (const std::string& word : variationListObj["wordList"]) {
                    variationList.wordList.push_back(word);
                    variationList.unparsedString += (word + "\n");
                }

                variationList.shuffledWordList = ShuffleWordList(variationList.wordList);

                Variations.push_back(variationList);
            }
        }
    }
}
