
void
AddEmptyBinding();

void
DeleteBinding(int idx);

void
InitKeyStates();
int selectedBindingIndex = 0;
std::unordered_map<std::string, bool> keyStates;
