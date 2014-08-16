#ifndef TIBIA_OPTIONSWINDOW_HPP
#define TIBIA_OPTIONSWINDOW_HPP

#include "iup.h"

#include "tibia/Tibia.hpp"
#include "tibia/Utility.hpp"

namespace tibia
{

namespace OptionsWindow
{
    Ihandle* dialog;
    Ihandle* button_ok;

    void button_ok_action_callback()
    {
        IupHide(dialog);
    }

    void load()
    {
        button_ok = IupButton("OK", NULL);
        IupSetHandle("OptionsWindow_button_ok", button_ok);
        IupSetCallback(button_ok, "ACTION", (Icallback)button_ok_action_callback);

        dialog = IupDialog(IupHbox(button_ok, NULL));
        IupSetAttribute(dialog, "TITLE", "Options");
        IupSetAttribute(dialog, "DEFAULTESC", "OptionsWindow_button_ok");
        IupSetAttribute(dialog, "SIZE", "320x240");
        IupSetAttribute(dialog, "MAXBOX", "NO");
        IupSetAttribute(dialog, "TOPMOST", "YES");
    }

    void show()
    {
        //IupShowXY(dialog, IUP_CENTER, IUP_CENTER);

        //IupSetAttribute(dialog, "BRINGFRONT", "YES");

        IupGetParam
        (
            "Options", NULL, 0,

            "Cheats %t\n"
                "Infinite Health: %b[Disabled,Enabled]\n",
                "Infinite Mana: %b[Disabled,Enabled]\n",

            &Game::getInstance().options.isInfiniteHealthEnabled,
            &Game::getInstance().options.isInfiniteManaEnabled,

            NULL
        );
    }
}

} // namespace tibia

#endif // TIBIA_OPTIONSWINDOW_HPP
