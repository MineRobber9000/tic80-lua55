// MIT License

// Copyright (c) 2017 Vadim Grigoruk @nesbox // grigoruk@gmail.com

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "core/core.h"
#include "luaapi.h"

#include <stdlib.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <ctype.h>

static bool initLua(tic_mem* tic, const char* code)
{
    tic_core* core = (tic_core*)tic;

    luaapi_close(tic);

    lua_State* lua = core->currentVM = luaL_newstate();
    // Load all libraries except io and os (base TIC-80 doesn't load utf8 for some reason?)
    int load_mask = ~0 ^ LUA_IOLIBK ^ LUA_OSLIBK;
    luaL_openselectedlibs(lua, load_mask, 0);

    luaapi_init(core);

    {
        lua_State* vm = core->currentVM;

        lua_settop(vm, 0);

        if(luaL_loadstring(vm, code) != LUA_OK || lua_pcall(vm, 0, LUA_MULTRET, 0) != LUA_OK)
        {
            core->data->error(core->data->data, lua_tostring(vm, -1));
            return false;
        }
    }

    return true;
}

static const char* const LuaKeywords[] =
    {
        "and", "break", "do", "else", "elseif",
        "end", "false", "for", "function", "goto", "if",
        "in", "local", "nil", "not", "or", "repeat",
        "return", "then", "true", "until", "while",
        "self"};

static inline bool isalnum_(char c)
{
    return isalnum((u8)c) || c == '_';
}

static const tic_outline_item* getLuaOutline(const char* code, s32* size)
{
    enum
    {
        Size = sizeof(tic_outline_item)
    };

    *size = 0;

    static tic_outline_item* items = NULL;

    if(items)
    {
        free(items);
        items = NULL;
    }

    const char* ptr = code;

    while(true)
    {
        static const char FuncString[] = "function ";

        ptr = strstr(ptr, FuncString);

        if(ptr)
        {
            ptr += sizeof FuncString - 1;

            const char* start = ptr;
            const char* end = start;

            while(*ptr)
            {
                char c = *ptr;

                if(isalnum_(c) || c == ':' || c == '.')
                    ;
                else if(c == '(')
                {
                    end = ptr;
                    break;
                }
                else
                    break;

                ptr++;
            }

            if(end > start)
            {
                items = realloc(items, (*size + 1) * Size);

                items[*size].pos = start;
                items[*size].size = (s32)(end - start);

                (*size)++;
            }
        }
        else
            break;
    }

    return items;
}

static void evalLua(tic_mem* tic, const char* code)
{
    tic_core* core = (tic_core*)tic;
    lua_State* lua = core->currentVM;

    if(!lua) return;

    lua_settop(lua, 0);

    if(luaL_loadstring(lua, code) != LUA_OK || lua_pcall(lua, 0, LUA_MULTRET, 0) != LUA_OK)
    {
        core->data->error(core->data->data, lua_tostring(lua, -1));
    }
}

static const u8 DemoRom[] =
    {
#include "../build/assets/luademo.tic.dat"
};

static const u8 MarkRom[] =
    {
#include "../build/assets/luamark.tic.dat"
};

TIC_EXPORT const tic_script EXPORT_SCRIPT(Lua55) =
    {
        .id = 155,
        .name = "lua55",
        .fileExtension = ".lua",
        .projectComment = "--",
        {
            .init = initLua,
            .close = luaapi_close,
            .tick = luaapi_tick,
            .boot = luaapi_boot,

            .callback =
                {
                    .scanline = luaapi_scn,
                    .border = luaapi_bdr,
                    .menu = luaapi_menu,
                },
        },

        .getOutline = getLuaOutline,
        .eval = evalLua,

        .blockCommentStart = "--[[",
        .blockCommentEnd = "]]",
        .blockCommentStart2 = NULL,
        .blockCommentEnd2 = NULL,
        .singleComment = "--",
        .blockStringStart = "[[",
        .blockStringEnd = "]]",
        .stdStringStartEnd = "\'\"",
        .blockEnd = "end",

        .keywords = LuaKeywords,
        .keywordsCount = COUNT_OF(LuaKeywords),

        .demo = {DemoRom, sizeof DemoRom, NULL},
        .mark = {MarkRom, sizeof MarkRom, "luamark.tic"},

};
