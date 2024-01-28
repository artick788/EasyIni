#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <algorithm>
#include <filesystem>
#include <iostream>


namespace EasyIni{

    template <typename T>
    struct IsCharArray : std::false_type {};

    template <typename CharT, std::size_t Size>
    struct IsCharArray<CharT[Size]> : std::true_type {};

    template <typename T>
    constexpr bool is_char_array = IsCharArray<T>::value;

    class Element{
    public:
        Element() = delete;

        explicit Element(const std::string& key, const std::string& value):
        m_Key(key),
        m_Value(value){
        }

        ~Element() = default;

        void save(std::ofstream& file) const {
            // no need to add spaces around the '=' character since we didn't remove them
            file << m_Key << "=" << m_Value << std::endl;
        }

        template<typename T>
        void operator=(const T& value){
            std::stringstream ss;
            if constexpr (std::is_same_v<T, std::string> or
                          is_char_array<T> or
                          std::is_same_v<T, const char*> or
                          std::is_same_v<T, char*> or
                          std::is_same_v<T, const char (&)[]> or
                          std::is_same_v<T, char (&)[]>){
                ss << std::quoted(value);
            } else {
                ss << value;
            }
            m_Value = ss.str();
        }

        template<typename T>
        void operator=(const std::vector<T>& value){
            std::stringstream ss;
            ss << "(";
            for (size_t i = 0; i < value.size(); ++i){
                if constexpr (std::is_same_v<T, std::string> or
                              is_char_array<T> or
                              std::is_same_v<T, const char*> or
                              std::is_same_v<T, char*> or
                              std::is_same_v<T, const char (&)[]> or
                              std::is_same_v<T, char (&)[]>){
                    ss << std::quoted(value[i]);
                } else {
                    ss << value[i];
                }
                if (i != value.size() - 1){
                    ss << ",";
                }
            }
            ss << ")";
            m_Value = ss.str();
        }

        template<typename T>
        inline T get(){
            std::stringstream ss(m_Value);
            T t;
            ss >> t;
            return t;
        }

        template<typename T>
        inline T getOrDefault(T& def){
            if (m_Value.empty()){
                return def;
            }
            return get<T>();
        }

        template<typename T>
        std::vector<T> getVector(){
            auto cVal = m_Value;
            cVal.erase(std::remove(cVal.begin(), cVal.end(), ' '), cVal.end());
            cVal.erase(std::remove(cVal.begin(), cVal.end(), '\t'), cVal.end());
            cVal.erase(std::remove(cVal.begin(), cVal.end(), '('), cVal.end());
            cVal.erase(std::remove(cVal.begin(), cVal.end(), ')'), cVal.end());
            std::vector<T> result;
            std::stringstream ss(cVal);
            std::string item;
            while (std::getline(ss, item, ',')){
                std::stringstream ssItem(item);
                T t;
                ssItem >> t;
                result.emplace_back(t);
            }
            return result;
        }

        template<typename T>
        std::vector<T> getVectorOrDefault(std::vector<T>& def){
            if (m_Value.empty()){
                return def;
            }
            return getVector<T>();
        }

    private:
        std::string m_Key;
        std::string m_Value;

    };

    template<>
    inline std::string Element::get<std::string>(){
        std::string result;
        std::stringstream ss(m_Value);
        ss >> std::quoted(result);
        return result;
    }

    template<>
    std::vector<std::string> Element::getVector<std::string>(){
        auto cVal = m_Value;
        cVal.erase(std::remove(cVal.begin(), cVal.end(), '('), cVal.end());
        cVal.erase(std::remove(cVal.begin(), cVal.end(), ')'), cVal.end());
        std::vector<std::string> result;
        std::stringstream ss(cVal);
        std::string item;
        while (std::getline(ss, item, ',')){
            std::stringstream ssItem(item);
            std::string t;
            ssItem >> std::quoted(t);
            result.emplace_back(t);
        }
        return result;
    }

    class Section{
    public:
        Section() = delete;

        explicit Section(const std::string& name):
        m_Name(name){

        }

        void addElement(const std::string& key, const std::string& value){
            m_Elements.emplace(key, Element(key, value));
        }

        Element& operator[](const std::string& key){
            if (m_Elements.find(key) == m_Elements.end()){
                m_Elements.emplace(key, Element(key, ""));
            }
            return m_Elements.at(key);
        }

        void save(std::ofstream& file) const {
            for (const auto& [key, element] : m_Elements){
                element.save(file);
            }
        }

    private:
        std::string m_Name;
        std::unordered_map<std::string, Element> m_Elements;

    };

    class Configuration{
    public:
        Configuration() = delete;

        explicit Configuration(const std::string& iniFile):
        m_IniFile(iniFile){
            if (!std::filesystem::exists(iniFile)){
                throw std::runtime_error("File does not exist");
            }
            std::ifstream file(iniFile);
            if (!file.is_open()){
                throw std::runtime_error("Could not open file");
            }
            std::string line;
            Section* currentSection = nullptr;
            while (std::getline(file, line)){
                if (line.empty()){
                    continue;
                }
                if (line[0] == ';' or line[0] == '#'){
                    continue;
                }
                auto cBracketOpen = line.find('[');
                auto cBracketClose = line.find(']');
                if (cBracketOpen != std::string::npos and cBracketClose != std::string::npos){
                    auto sectionName = line.substr(cBracketOpen + 1, cBracketClose - cBracketOpen - 1);
                    m_Sections.emplace(sectionName, Section(sectionName));
                    currentSection = &m_Sections.at(sectionName);
                    continue;
                }
                auto assignPos = line.find('=');
                if (assignPos != std::string::npos){
                    auto key = line.substr(0, assignPos);
                    key.erase(std::remove(key.begin(), key.end(), ' '), key.end());
                    key.erase(std::remove(key.begin(), key.end(), '\t'), key.end());
                    key.erase(std::remove(key.begin(), key.end(), '\r'), key.end());
                    key.erase(std::remove(key.begin(), key.end(), '\n'), key.end());
                    auto value = line.substr(assignPos + 1);
                    if (currentSection != nullptr){
                        currentSection->addElement(key, value);
                    }
                }
            }
        }

        ~Configuration() = default;

        void save(){
            std::ofstream file(m_IniFile);
            if (!file.is_open()){
                throw std::runtime_error("Could not open file");
            }
            for (const auto& [sectionName, section] : m_Sections){
                file << "[" << sectionName<< "]" << std::endl;
                section.save(file);
                file << std::endl;
            }
        }

        void save(const std::string& iniFile){
            m_IniFile = iniFile;
            save();
        }

        Section& operator[](const std::string& sectionName){
            if (m_Sections.find(sectionName) == m_Sections.end()){
                m_Sections.emplace(sectionName, Section(sectionName));
            }
            return m_Sections.at(sectionName);
        }

        Section& getSection(const std::string& sectionName){
            if (m_Sections.find(sectionName) == m_Sections.end()){
                throw std::runtime_error("Section: " + sectionName + " does not exist");
            }
            return m_Sections.at(sectionName);
        }

        const Section& getSection(const std::string& sectionName) const{
            if (m_Sections.find(sectionName) == m_Sections.end()){
                throw std::runtime_error("Section: " + sectionName + " does not exist");
            }
            return m_Sections.at(sectionName);
        }

        void addSection(const std::string& sectionName){
            m_Sections.emplace(sectionName, Section(sectionName));
        }

    private:
        std::string m_IniFile;
        std::unordered_map<std::string, Section> m_Sections;

    };


}