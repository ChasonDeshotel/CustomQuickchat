#pragma once

class Variations {
  public:
    void AddEmptyVariationList();
    std::vector<VariationList> getVariations();

    void DeleteVariationList(int idx);
    void UpdateDataFromVariationStr();

    std::string Variation(const std::string& listName);

  private:
    std::vector<VariationList> Variations;
    int selectedVariationIndex = 0;
    std::vector<std::string> ShuffleWordList(const std::vector<std::string>& ogList);
    void ReshuffleWordList(int idx);
};