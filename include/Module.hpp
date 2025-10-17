#pragma once

enum class ModuleCategory {
    Gameplay = 0,
    Editor = 1,
    Bypass = 2,
    Informational = 3,
    Universal = 4
};

class Module {
public:
    // const char* id;
    const char* name;
    std::string description;
    bool enabled;
    ModuleCategory category;
    void (*onToggle)(bool);

    Module(const char* name, std::string description, bool defaultEnabled, ModuleCategory category, void (*onToggle)(bool)) {
        // this->id = id;
        this->name = name;
        this->description = description;
        this->enabled = defaultEnabled;
        this->category = category;
        this->onToggle = onToggle;
    }

    bool isEnabled() {
        return enabled;
    }
    void setEnabled(bool value) {
        enabled = value;
        onToggle(enabled);
    }
    void toggle() {
        setEnabled(!enabled);
    }
};