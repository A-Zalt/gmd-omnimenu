#pragma once
#include "Module.hpp"

template <typename T> class Preference {
public:
    bool exists;
    std::string id;
    const char* name;
    Module parent;
protected:
    T value;
    T defaultValue;
public:

    Preference() = default;

    Preference(std::string id,
           const char* name,
           Module parent,
           T defaultValue)
        : exists(true), // only when explicitly constructed
          id(std::move(id)),
          name(name),
          parent(parent),
          value(defaultValue),
          defaultValue(defaultValue) {}
    
    T getValue() {
        return value;
    }

    void setValue(T newValue) {
        value = newValue;
    }

    void resetValue() {
        value = defaultValue;
    }
};