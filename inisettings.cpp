#include <algorithm>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <string>

#include "inisettings.h"

IniSettings::IniSettings(const char *path):
    m_path(path),
    m_groupBegin(-1),
    m_groupEnd(-1)
{
    load(path);
}

IniSettings::~IniSettings()
{
    save();
}

void IniSettings::load(const char *path)
{
    std::string line;

    m_data.clear();
    m_path = std::string(path);
    std::ifstream myfile(path);

    if (myfile.is_open())
    {
        while (std::getline(myfile, line))
            m_data.push_back(line);

        myfile.close();
    }
}

void IniSettings::save()
{
    if (!m_changed) return;

    std::ofstream ofs(m_path);

    if (ofs.is_open())
    {
        for(auto it = m_data.begin(); it != m_data.end(); ++it)
            ofs << *it << std::endl;

        ofs.close();
    }

    m_changed = false;
}

void IniSettings::beginGroup(const char *group)
{
    m_group = std::string(group);

    if (m_group.empty())
        m_group = std::string("GENERAL");

    std::string line, key, value;
    line.reserve(20);
    key.reserve(20);
    value.reserve(20);

    m_map.clear();
    m_groupBegin = -1;
    m_groupEnd = -1;

    // find line with same group name
    for(size_t i=0; i<m_data.size(); ++i)
    {
        line = m_data[i];

        if (line.size() < 3) continue;

        // if group is found
        if (m_groupBegin >= 0)
        {
            // if group is closed;
            if (line[0] == '[')
            {
                break;
            }
            else
            {
                ++m_groupEnd;

                for (size_t j=0; j<line.size(); ++j)
                {
                    if (line[j] == '=')
                    {
                        m_map[line.substr(0, j)] = line.substr(j + 1, line.size() - (j + 1));
                        break;
                    }
                }
            }
        }
        else
        {
            if (line[0] == '[')
            {
                if (line.size() < m_group.size() + 2) continue;
                if (line[m_group.size() + 1] != ']') continue;
                if (line.compare(1, m_group.size(), m_group) != 0) continue;

                m_groupBegin = static_cast<int>(i);
                m_groupEnd = m_groupBegin;
            }
        }
    }
}

void IniSettings::clearGroup()
{
    m_map.clear();
    m_changed = true;
}

void IniSettings::endGroup()
{
    if (!m_changed) return;

    if (m_map.empty())
    {
        m_data.erase(m_data.begin() + m_groupBegin, m_data.begin() + m_groupEnd + 1);
        return;
    }

    std::vector<std::string> groupData;
    groupData.reserve(m_map.size() + 2);

    std::string line;
    line.reserve(20);

    line += "["; line += m_group; line += "]";
    groupData.push_back(line);

    for (auto it = m_map.begin(); it != m_map.end(); ++it)
    {
        line.clear();
        line += it->first; line += "="; line += it->second;
        groupData.push_back(line);
    }

    int groupSize = m_groupEnd - m_groupBegin + 1;
    int diff = groupData.size() - groupSize;

    if (diff == 0)
    {
        std::copy(groupData.begin(), groupData.end(), m_data.begin() + m_groupBegin);
    }
    else if(diff > 0)
    {
        // if group not found
        if (m_groupBegin == -1)
        {
            groupData.push_back("");
            m_data.insert(m_data.end(), groupData.begin(), groupData.end());
        }
        else
        {
            std::copy(groupData.begin(), groupData.begin() + groupSize, m_data.begin() + m_groupBegin);
            m_data.insert(m_data.begin() + m_groupBegin + groupSize, groupData.begin() + groupSize, groupData.end());
        }
    }
    else if(diff < 0)
    {
        std::copy(groupData.begin(), groupData.end(), m_data.begin() + m_groupBegin);
        m_data.erase(m_data.begin() + m_groupBegin + groupData.size(), m_data.begin() + m_groupEnd + 1);
    }
}

void IniSettings::setValue(const char *key, const char *value)
{
    m_map[std::string(key)] = std::string(value);
    m_changed = true;
}

void IniSettings::setValue(const char *key, const std::string &value)
{
    m_map[std::string(key)] = value;
    m_changed = true;
}

void IniSettings::setValue(const char *key, int value)
{
    m_map[std::string(key)] = std::to_string(value);
    m_changed = true;
}

void IniSettings::setValue(const char *key, double value)
{
    std::ostringstream streamObj;
    streamObj << std::fixed << std::setprecision(20) << value;
    m_map[std::string(key)] = streamObj.str();
    m_changed = true;
}

std::string IniSettings::value(const char *key)
{
    auto it = m_map.find(key);

    if (it != m_map.end()) return it->second;
    else return std::string();
}

std::string IniSettings::value(const char *key, const std::string &_default)
{
    auto it = m_map.find(key);

    if (it != m_map.end()) return it->second;
    else return _default;
}

int IniSettings::value(const char *key, int _default)
{
    auto it = m_map.find(key);

    if (it != m_map.end()) return std::stoi(it->second);
    else return _default;
}

double IniSettings::value(const char *key, double _default)
{
    auto it = m_map.find(key);

    if (it != m_map.end()) return std::stod(it->second);
    else return _default;
}
