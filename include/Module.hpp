#pragma once

enum class ModuleCategory {
    Player = 0,
    Visual = 7,
    Editor = 1,
    Bypass = 2,
    Informational = 3,
    Universal = 4,
    Label = 5,
    Particles = 6,
    SaveData = 8 // not a real category, just exists for hax.lastCategory to remember the Save Data tab
};

// class Module {
// public:
//     const char* name;
//     std::string description;
//     bool enabled;
//     ModuleCategory category;
//     void (*onToggle)(bool);

//     Module(const char* name, std::string description, bool defaultEnabled, ModuleCategory category, void (*onToggle)(bool)) {
//         // this->id = id;
//         this->name = name;
//         this->description = description;
//         this->enabled = defaultEnabled;
//         this->category = category;
//         this->onToggle = onToggle;
//     }

//     bool isEnabled() {
//         return enabled;
//     }
//     void setEnabled(bool value) {
//         enabled = value;
//         onToggle(enabled);
//     }
//     void toggle() {
//         setEnabled(!enabled);
//     }
// };

class Module {
public:
    const char* id;
    const char* name;
    std::string description;
    bool enabled;
    ModuleCategory category;
    void (*onToggle)(bool);
    bool exists;

    Module() = default;

    Module(const char* id,
           const char* name,
           std::string description,
           bool defaultEnabled,
           ModuleCategory category,
           void (*onToggle)(bool))
        : exists(true), // only when explicitly constructed
          id(id),
          name(name),
          description(std::move(description)),
          enabled(defaultEnabled),
          category(category),
          onToggle(onToggle) {}
    
    void setEnabled(bool value) {
        enabled = value;
        if (onToggle) onToggle(value);
    }
    void toggle() {
        setEnabled(!enabled);
    }
};