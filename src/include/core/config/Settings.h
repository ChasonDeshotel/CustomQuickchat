#prama once

void
RenderSettings() override;
void
RenderWindow() override;

void
RenderAllBindings();
void
RenderBindingDetails();

void
RenderChatDetails(Binding& selectedBinding);
void
RenderBindingTriggerDetails(Binding& selectedBinding);

void
GeneralSettings();
void
ChatTimeoutSettings();
void
SpeechToTextSettings();
void
LastChatSettings();

// footer images
bool assets_exist = false;

static constexpr float header_height = 80.0f;
static constexpr float footer_height = 40.0f;
static constexpr float footer_img_height = 25.0f;

static constexpr const wchar_t* github_link = L"https://github.com/smallest-cock/CustomQuickchat";
static constexpr const char* github_link_tooltip = "See the sauce";

void
gui_footer_init();
std::shared_ptr<GUI::FooterLinks> footer_links;
