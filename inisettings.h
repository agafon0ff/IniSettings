#ifndef INISETTINGS_H
#define INISETTINGS_H

#include <string>
#include <vector>
#include <map>

class IniSettings
{
public:
    explicit IniSettings(const char* path);
    ~IniSettings();

    void load(const char* path);
    void save();

    void beginGroup(const char *group);
    void clearGroup();
    void endGroup();

    void setValue(const char *key, const char *value);
    void setValue(const char *key, const std::string &value);
    void setValue(const char *key, int value);
    void setValue(const char *key, double value);

    std::string value(const char *key);
    std::string value(const char *key, const std::string &_default);
    int         value(const char *key, int _default);
    double      value(const char *key, double _default);
    std::map<std::string, std::string> values(){ return m_map; }

private:
    std::string m_path;
    std::string m_group;
    int m_groupBegin, m_groupEnd;
    bool m_changed = false;

    std::vector<std::string> m_data;
    std::map<std::string, std::string> m_map;
};

#endif // INISETTINGS_H
