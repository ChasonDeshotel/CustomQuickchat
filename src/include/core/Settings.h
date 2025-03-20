#pragma once

class Settings {
  public:
    Settings(std::function<std::shared_ptr<FileUtil>()> fileUtil);
    ~Settings() = default;

    Settings(const Settings&) = delete;
    auto operator=(const Settings&) -> Settings& = delete;
    Settings(Settings&&) = delete;
    auto operator=(Settings&&) -> Settings& = delete;

    void init();

    std::vector<Binding> Bindings;
};