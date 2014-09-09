#ifndef TIBIA_LUAFUNCTIONS_HPP
#define TIBIA_LUAFUNCTIONS_HPP

#include "lua.hpp"

#include "tibia/Tibia.hpp"
#include "tibia/Utility.hpp"

namespace tibia
{

namespace LuaFunctions
{
    int setTileId(lua_State* L)
    {
        const int x = luaL_checkinteger(L, 1);
        const int y = luaL_checkinteger(L, 2);
        const int z = luaL_checkinteger(L, 3);

        const int id = luaL_checkinteger(L, 4);

        g_game.setTileId(x, y, z, id);

        return 0;
    }

    int setTileObjectId(lua_State* L)
    {
        const int x = luaL_checkinteger(L, 1);
        const int y = luaL_checkinteger(L, 2);
        const int z = luaL_checkinteger(L, 3);

        const int oldId = luaL_checkinteger(L, 4);
        const int newId = luaL_checkinteger(L, 5);

        g_game.setTileObjectId(x, y, z, oldId, newId);

        return 0;
    }

    int doPlayerAddInventoryItem(lua_State* L)
    {
        const int id    = luaL_checkinteger(L, 1);
        const int count = luaL_checkinteger(L, 2);

        g_game.doPlayerAddInventoryItem(id, count);

        return 0;
    }

    int doesPlayerHaveInventoryItem(lua_State* L)
    {
        const int id = luaL_checkinteger(L, 1);

        bool result = g_game.getPlayer()->hasInventoryItem(id);

        lua_pushinteger(L, result);

        return 1;
    }

    int showStatusBarText(lua_State* L)
    {
        const char* text = luaL_checkstring(L, 1);

        g_game.showStatusBarText(text);

        return 0;
    }

    int doChatLogWindowAddText(lua_State* L)
    {
        const char* text = luaL_checkstring(L, 1);

        g_game.doChatLogWindowAddText(text, sf::Color::Black);

        return 0;
    }

    void registerFunctions()
    {
        lua_register(g_luaState, "setTileId", setTileId);
        lua_register(g_luaState, "setTileObjectId", setTileObjectId);
        lua_register(g_luaState, "doPlayerAddInventoryItem", doPlayerAddInventoryItem);
        lua_register(g_luaState, "doesPlayerHaveInventoryItem", doesPlayerHaveInventoryItem);
        lua_register(g_luaState, "showStatusBarText", showStatusBarText);
        lua_register(g_luaState, "doChatLogWindowAddText", doChatLogWindowAddText);
    }
} // namespace LuaFunctions

} // namespace tibia

#endif // TIBIA_LUAFUNCTIONS_HPP
