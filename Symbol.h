#pragma once
#include <string>
#include <vector>

class SymbolTableClass{
    public:
        bool Exists(const std::string & s) const;

        void AddEntry(const std::string & s);

        int GetValue(const std::string & s) const;

        void SetValue(const std::string & s, int v);

        int GetIndex(const std::string & s) const;

        size_t GetCount() const;

        struct Variable{
            std::string mLabel;
            int mValue;
        };
    
    private:
        std::vector<Variable> mTable;
  
};