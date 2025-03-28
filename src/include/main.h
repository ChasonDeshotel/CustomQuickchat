#pragma once
#include "DependencyContainer.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"

#define _WINSOCKAPI_ // stops windows.h including winsock.h -- conflicts with websocketpp
// #define _WEBSOCKETPP_WP11_TYPE_TRAITS_

class CustomQuickchat : public BakkesMod::Plugin::BakkesModPlugin {
  public:
    CustomQuickchat();
    void onLoad() override;
    void onUnload() override;

  private:
    DependencyContainer& container_;
};