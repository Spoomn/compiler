#include "Symbol.h"
#include <stdexcept>

bool SymbolTableClass::Exists(const std::string & s) const {
    for (const auto & var : mTable) {
        if (var.mLabel == s) {
            return true;
        }
    }
    return false;
}

void SymbolTableClass::AddEntry(const std::string & s) {
    if (Exists(s)) {
        throw std::runtime_error("AddEntry Error: Symbol '" + s + "' already exists");
    }
    Variable var;
    var.mLabel = s;
    var.mValue = 0;
    mTable.push_back(var);
}

int SymbolTableClass::GetValue(const std::string & s) const {
    for (const auto & var : mTable) {
        if (var.mLabel == s) {
            return var.mValue;
        }
    }
    throw std::runtime_error("GetValue error: Symbol '" + s + "' not found.");
}

void SymbolTableClass::SetValue(const std::string & s, int v) {
    for (auto & var : mTable) {
        if (var.mLabel == s) {
            var.mValue = v;
            return;
        }
    }
    throw std::runtime_error("SetValue error: Symbol '" + s + "' not found.");
}

int SymbolTableClass::GetIndex(const std::string & s) const {
    for (size_t i = 0; i < mTable.size(); ++i) {
        if (mTable[i].mLabel == s) {
            return static_cast<int>(i);
        }
    }
    throw std::runtime_error("GetIndex error: Symbol '" + s + "' not found.");
}

size_t SymbolTableClass::GetCount() const {
    return mTable.size();
}