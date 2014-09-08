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

    void registerFunctions()
    {
        lua_register(g_luaState, "setTileId",       setTileId);
        lua_register(g_luaState, "setTileObjectId", setTileObjectId);
    }
} // namespace LuaFunctions

} // namespace tibia

#endif // TIBIA_LUAFUNCTIONS_HPP
