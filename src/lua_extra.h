#pragma once
#include <string>
#include <sstream>
#include <unordered_map>
#include <map>
#include "lua5.4/lua.hpp"

bool isNumber(const std::string &str)
{
	std::istringstream iss(str);
	double num;
	iss >> std::noskipws >> num;
	return iss.eof() && !iss.fail();
}

float stringToFloat(const std::string &str)
{
	std::istringstream iss(str);
	float result;
	iss >> result;
	return result;
}

std::string floatToString(float value)
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}

class Table
{
public:
    std::unordered_map<std::string, float> m_floatMap;
    std::unordered_map<std::string, std::string> m_stringMap;
    std::map<std::string, Table> m_tableMap;
    std::map<std::string, lua_CFunction> m_lua_functionMap;

    bool haveFloat(const std::string &key)
    {
        if (m_floatMap.find(key) != m_floatMap.end())
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool haveString(const std::string &key)
    {
        if (m_stringMap.find(key) != m_stringMap.end())
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool haveTable(const std::string &key)
    {
        if (m_tableMap.find(key) != m_tableMap.end())
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool haveLuaTable(const std::string &key)
    {
        if (m_lua_functionMap.find(key) != m_lua_functionMap.end())
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    void setFloat(const std::string &key, float value)
    {
        m_floatMap[key] = value;
    }

    float getFloat(const std::string &key)
    {
        return m_floatMap[key];
    }

    void setString(const std::string &key, const std::string &value)
    {
        m_stringMap[key] = value;
    }

    std::string getString(const std::string &key)
    {
        return m_stringMap[key];
    }

    void setTable(const std::string &key, const Table &table)
    {
        m_tableMap[key] = table;
    }

    Table getTable(const std::string &key)
    {
        return m_tableMap[key];
    }

    void setLuaFunction(const std::string &key, lua_CFunction &func)
    {
        m_lua_functionMap[key] = func;
    }

    lua_CFunction getLuaFunction(const std::string &key)
    {
        return m_lua_functionMap[key];
    }
};

void lua_pushtable(lua_State *L, Table t)
{
	lua_newtable(L);
	for (std::pair<std::string, float> p : t.m_floatMap)
	{
		if (isNumber(p.first))
		{
			lua_pushnumber(L, stringToFloat(p.first));
		}
		else
		{
			lua_pushstring(L, p.first.c_str());
		}
		lua_pushnumber(L, p.second);
		lua_settable(L, -3);
	}
	for (std::pair<std::string, std::string> p : t.m_stringMap)
	{
		if (isNumber(p.first))
		{
			lua_pushnumber(L, stringToFloat(p.first));
		}
		else
		{
			lua_pushstring(L, p.first.c_str());
		}
		lua_pushstring(L, p.second.c_str());
		lua_settable(L, -3);
	}
	for (std::pair<std::string, lua_CFunction> p : t.m_lua_functionMap)
	{
		if (isNumber(p.first))
		{
			lua_pushnumber(L, stringToFloat(p.first));
		}
		else
		{
			lua_pushstring(L, p.first.c_str());
		}
		lua_pushcfunction(L, p.second);
		lua_settable(L, -3);
	}
	for (std::pair<std::string, Table> p : t.m_tableMap)
	{
		if (isNumber(p.first))
		{
			lua_pushnumber(L, stringToFloat(p.first));
		}
		else
		{
			lua_pushstring(L, p.first.c_str());
		}
		lua_pushtable(L, p.second);
		lua_settable(L, -3);
	}
}

Table lua_totable(lua_State *L, int index)
{
	Table t;

	// Iterate over the table and extract its keys and values
	lua_pushnil(L); // Push the first key
	while (lua_next(L, index) != 0)
	{
		std::string key;
		// At this point, the stack contains the key at index -2 and the value at index -1

		if (lua_type(L, -2) == LUA_TNUMBER)
		{
			// The value is a number
			float value = lua_tonumber(L, -2);
			key = floatToString(value);
		}
		else if (lua_type(L, -2) == LUA_TSTRING)
		{
			// The value is a number
			std::string value = lua_tostring(L, -2);
			key = value;
		}
		else if (lua_type(L, -2) == LUA_TBOOLEAN)
		{
			// The value is a number
			float value = lua_toboolean(L, -2);
			key = std::to_string(value);
		}

		if (lua_type(L, -1) == LUA_TNUMBER)
		{
			// The value is a number
			float value = lua_tonumber(L, -1);
			t.setFloat(key, value);
		}
		else if (lua_type(L, -1) == LUA_TSTRING)
		{
			// The value is a number
			std::string value = lua_tostring(L, -1);
			t.setString(key, value);
		}
		else if (lua_type(L, -1) == LUA_TBOOLEAN)
		{
			// The value is a number
			float value = lua_toboolean(L, -1);
			t.setFloat(key, value);
		}
		else if (lua_type(L, -1) == LUA_TFUNCTION)
		{
			// The value is a number
			lua_CFunction value = lua_tocfunction(L, -1);
			t.setLuaFunction(key, value);
		}
		else if (lua_type(L, -1) == LUA_TTABLE)
		{
			// The value is a table, recurse into it
			t.setTable(key, lua_totable(L, lua_gettop(L)));
		}
		// Pop the value, but leave the key for the next iteration
		lua_pop(L, 1);
	}
	return t;
}


