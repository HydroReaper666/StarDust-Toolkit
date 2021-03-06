/*
* StarDust
* Copyright (C) 2018  Team ReiSwitched
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <switch.h>
#include "UI.hpp"
#include <iostream>
#include <ctime>
#include <cstdlib>
using namespace std;
u32 clock_count = 0;

int main() {    
	UI ui(TITLE, VERSION);
	UI::setInstance(ui);
	ui.InitialStage();
    ui.renderMenu();
    
    while(appletMainLoop()) {

		
		ui.InitialStage();
        hidScanInput();
		//menu refresh speed
		clock_count++;
		if (clock_count == 100000) {
		ui.renderMenu();
		clock_count = 0;
		}
		
        u64 PressedInput = hidKeysDown(CONTROLLER_P1_AUTO);
        if((PressedInput & KEY_LSTICK_UP)||(PressedInput & KEY_DUP)) {
            ui.inSubMenu ? ui.SubMenuUp() : ui.MenuUp();
            ui.renderMenu();
        }
        else if((PressedInput & KEY_LSTICK_DOWN)||(PressedInput & KEY_DDOWN)) {
            ui.inSubMenu ? ui.SubMenuDown() : ui.MenuDown();
            ui.renderMenu();
        }
        if(PressedInput & KEY_A) {
            ui.MenuSel();
            ui.renderMenu();
        }
        if(PressedInput & KEY_B) {
            ui.MenuBack();
            ui.renderMenu();
        }
        if(PressedInput & KEY_PLUS) ui.exitApp();
		
        if(PressedInput & KEY_X) {
            ui.optGetPatch();
        }
		if(PressedInput & KEY_MINUS) {
            ui.opttemplate();
        }
    }
}