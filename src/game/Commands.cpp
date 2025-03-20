#include "../include/main.h"

void
CustomQuickchat::cmd_toggleEnabled(std::vector<std::string> args) {
    CVarWrapper enabledCvar = GetCvar(Cvars::enabled);
    if (!enabledCvar)
        return;

    bool enabled = enabledCvar.getBoolValue();
    enabledCvar.setValue(!enabled);
}

void
CustomQuickchat::cmd_list_custom_chat_labels(std::vector<std::string> args) {
    determine_quickchat_labels(nullptr, true);

    LOG("============= pc custom qc lablels ============");

    for (int i = 0; i < 4; i++) {
        const auto& chat_label_arr = pc_qc_labels[i];

        LOG("{}:", preset_group_names[i]);
        for (int j = 0; j < 4; j++) {
            LOG("[{}]\t{}", j, chat_label_arr.at(j).ToString());
        }
    }

    LOG("=========== gamepad custom qc lablels =========");
    for (int i = 0; i < 4; i++) {
        const auto& chat_label_arr = gp_qc_labels[i];

        LOG("{}:", preset_group_names[i]);
        for (int j = 0; j < 4; j++) {
            LOG("[{}]\t{}", j, chat_label_arr.at(j).ToString());
        }
    }
}

void
CustomQuickchat::cmd_list_playlist_info(std::vector<std::string> args) {
    UOnlineGamePlaylists_X* playlists = Instances.GetInstanceOf<UOnlineGamePlaylists_X>();
    if (!playlists)
        return;

    LOG("DownloadedPlaylists size: {}", playlists->DownloadedPlaylists.size());

    LOG("--------------------------------------");
    LOG("ID --> Internal name --> Display name");
    LOG("--------------------------------------");
    for (const auto& p : playlists->DownloadedPlaylists) {
        if (!p)
            continue;

        LOG("{} --> {} --> {}", p->PlaylistId, playlists->IdToName(p->PlaylistId).ToString(), p->GetLocalizedName().ToString());
    }
}
