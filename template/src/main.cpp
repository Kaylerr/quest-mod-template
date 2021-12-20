#include "main.hpp"

static ModInfo modInfo; // Stores the ID and version of our mod, and is sent to the modloader upon startup

// Loads the config from disk using our modInfo, then returns it for use
Configuration& getConfig() {
    static Configuration config(modInfo);
    config.Load();
    return config;
}

// Returns a logger, useful for printing debug messages
Logger& getLogger() {
    static Logger* logger = new Logger(modInfo);
    return *logger;
}

// Called at the early stages of game loading
extern "C" void setup(ModInfo& info) {
    info.id = ID;
    info.version = VERSION;
    modInfo = info;
	
    getConfig().Load(); // Load the config file
    getLogger().info("Completed setup!");
}

// Called later on in the game loading - a good time to install function hooks
extern "C" void load() {
    il2cpp_functions::Init();

    getLogger().info("Installing hooks...");
    // Install our hooks (none defined yet)
    getLogger().info("Installed all hooks!");
}

std::optional<UnityEngine::GameObject*> FindComponentWithText(std::string_view find_text) {
  // find root components in scene
  auto scene = UnityEngine::SceneManager::GetActiveScene();
  auto root_objs = scene->GetRootGameObjects();
  for (int i = 0; i < root_objs->Length(); i++) {
    auto text_components = root_objs->values[i]->GetComponentsInChildren<HMUI::CurvedTextMeshPro>();
    for (int j = 0; j < text_components->Length(); j++) {
      auto text = to_utf8(csstrtostr(text_components->values[i]->get_text()));
      if (text == find_text) {
        return text_components->values[i]->get_gameObject();
      }
    }
  }
  return std::nullopt;

#include "GlobalNamespace/MainMenuViewController.hpp"
#include "HMUI/CurvedTextMeshPro.hpp"
MAKE_HOOK_MATCH(MainMenuViewController_DidActivate, &GlobalNamespace::MainMenuViewController::DidActivate, void,
  GlobalNamespace::MainMenuViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling
) {
  MainMenuViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
  auto solo_button = UnityEngine::GameObject::Find(il2cpp_utils::createcsstr("SoloButton"));
  auto solo_text = solo_button->FindComponent<HMUI::CurvedTextMeshPro>();
  solo_text->set_text("The mod worked... right?");
}
extern "C" void load() {
  il2cpp_functions::Init();
  INSTALL_HOOK(
    getLogger(),
    MainMenuViewController_DidActivate
  );
}



