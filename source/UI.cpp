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

#include <iostream>
#include <ctime>
#include <switch.h>
#include <fstream>
#include <string>
#include "Net/Request.hpp"
#include "Tools/autorcm.hpp"
#include "Tools/nandDump.hpp"
#include "Utils/unzip_utils.hpp"
#include "FS.hpp"
#include "UI.hpp"
#include "Power.hpp"

#include <stdio.h>
#include <unistd.h>

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720
using namespace std;
//string jujer = "hey";


static u32 currSel = 0;
static u32 currSubSel = 0;
static u32 vol = 64;
static u32 titleX = 60;
static u32 titleY = 30;
static u32 menuX = 55, menuY = 115;
static u32 subX = 411, subY = 88;



// Input vars, updated every iteration of the loop, proper way to access input using CustomUI
static u64 HeldInput = 0;
static u64 PressedInput = 0;
static u64 ReleasedInput = 0;


static string title;
static string version;
static string current_StarDust_version= "----";
string origen;
string destino;
string new_release = "";
string change_StarDust_net = "";
string twick = "----------------";
string StarDust_Autoboot = "";
string HBnew_release = "";
string UpdateSDS = "Install StarDust";
string rester = "";
string vernx;
u32 dev_count = 1;
u32 render_count = 0;
u32 rig_count = 1;
u32 clippy = 0;
u32 about = 0;
u32 keystrick = 0;
u32 tempcou = 0;

vector<MenuOption> mainMenu;
vector<SDL_Surface*> images;
Mix_Music *menuSel;
Mix_Music *menuConfirm;
Mix_Music *menuBack;

UI * UI::mInstance = 0;





/**
Thanks to PricelessTwo2
**/
/*
Ver ** 
*/
string SwitchIdent_GetFirmwareVersion(void) {
    setsysInitialize();
	Result ret = 0;
    SetSysFirmwareVersion ver;

    if (R_FAILED(ret = setsysGetFirmwareVersion(&ver))) {
	
        printf("setsysGetFirmwareVersion() failed: 0x%x.\n\n", ret);
		
		std:string mosca = "NO VA a joderse";
		return mosca;
    }
	static char major[9];
    snprintf(major, 19, "%u", ver.major);
	rester = std::string(major);
	
    static char buf[9];
	snprintf(buf, 19, "%u.%u.%u-%u%u", ver.major, ver.minor, ver.micro, ver.revision_major, ver.revision_minor);
        std::string switchver = std::string(buf);
		setsysExit();
    return switchver;
}



/*
* copy function
*/
bool copy_me(string origen, string destino) {
    clock_t start, end;
    start = clock();
    ifstream source(origen, ios::binary);
    ofstream dest(destino, ios::binary);

    dest << source.rdbuf();

    source.close();
    dest.close();

    end = clock();

    cout << "CLOCKS_PER_SEC " << CLOCKS_PER_SEC << "\n";
    cout << "CPU-TIME START " << start << "\n";
    cout << "CPU-TIME END " << end << "\n";
    cout << "CPU-TIME END - START " <<  end - start << "\n";
    cout << "TIME(SEC) " << static_cast<double>(end - start) / CLOCKS_PER_SEC << "\n";
	return 0;
}
/*
* copy folder function
*/



/* ---------------------------------------------------------------------------------------
* Menu functions
*/
//Update StarDustCFWpack
void UI::optStarDustUpdate() {
    ProgBar prog;
    prog.max = 5;
    prog.step = 1;
    string latest_release_url = "http://arte-tian-cuba.000webhostapp.com/net/StarDustCFWpack-ver.php";
    CreateProgressBar(&prog, "Updating StarDust...");
	
    string config_path = "sdmc:/StarDust/StarDustV.txt";
    std::ifstream file(config_path.c_str());
    if (file.is_open()) {
        std::string line;
        getline(file, current_StarDust_version);
        file.close();
    }
    
    Net net = Net();
    hidScanInput();
	
    string new_release = "";
    new_release = net.Request("GET",latest_release_url);
    new_release = net.readBuffer;
	net.readBuffer = "";
    string filename = "/StarDust_"+new_release+".zip";
    string url = "http://arte-tian-cuba.000webhostapp.com/net/StarDustCFWpack-url.php";

if(current_StarDust_version != new_release)
{

if(MessageBox("Update","The last release is_"+new_release+"_you have_"+current_StarDust_version+"\nThis download StarDust to microSD.\n\n\nUpdate?", TYPE_YES_NO)) 
{
	Net net = Net();
    hidScanInput();
    url = net.Request("GET",url);
    url = net.readBuffer;
	net.readBuffer = "";
	
    IncrementProgressBar(&prog);
    bool res = net.Download(url, filename);
    IncrementProgressBar(&prog);
	appletBeginBlockingHomeButton(0);
    if(!res){
		if (tempcou == 1) {
			rename("/atmosphere/titles/0100000000001000", "/atmosphere/titles/titbackup");
			rename("/ReiNX/titles/0100000000001000", "/ReiNX/titles/titbackup");
			rename("/sxos/titles/0100000000001000", "/sxos/titles/titbackup");
			}
	
	
        
        unzFile zip = Utils::zip_open(filename.c_str()); IncrementProgressBar(&prog);
        Utils::zip_extract_all(zip, "/"); IncrementProgressBar(&prog);
        Utils::zip_close(zip); IncrementProgressBar(&prog);
        remove(filename.c_str());
		
		if (tempcou == 1) {
		rename("/atmosphere/titles/0100000000001000", "/atmosphere/titles/titdelete");
		rename("/ReiNX/titles/0100000000001000", "/ReiNX/titles/titdelete");
		rename("/sxos/titles/0100000000001000", "/sxos/titles/titdelete");
		
		rename("/atmosphere/titles/titbackup", "/atmosphere/titles/0100000000001000");
		rename("/ReiNX/titles/titbackup", "/ReiNX/titles/0100000000001000");
		rename("/sxos/titles/titbackup", "/sxos/titles/0100000000001000");

		FS::DeleteDirRecursive("./atmosphere/titles/titdelete");
		FS::DeleteDirRecursive("./ReiNX/titles/titdelete");
		FS::DeleteDirRecursive("./sxos/titles/titdelete");
		}


		
	remove("/StarDust/StarDustV.txt");										//write new vercion
	std::ofstream notes("sdmc:/StarDust/StarDustV.txt", std::ios::app);		//write new vercion
    notes << new_release;													//write new vercion
	notes.close();															//write new vercion
		appletEndBlockingHomeButton();
		if(MessageBox("Update", "Update"" "+new_release+" ""has downloaded successfully!-.-\n\nDid you like to Reeboot Now", TYPE_YES_NO)) {
		UI::deinit();
		Power::Shutdown();
		}

    }else{
        prog.curr = 4;
		appletEndBlockingHomeButton();
        MessageBox("UpdateError", "Ahahahahahah Update Error!", TYPE_OK);
    }
 }
}
else
{
	appletEndBlockingHomeButton();
    MessageBox(
        "You're up to date",
        "The last release is"" "+new_release+" ""you have"" "+current_StarDust_version+"",
    TYPE_OK);
}
}

//keys switch
void UI::optgetkeys() {
    ProgBar prog;
    prog.max = 1;
    prog.step = 1;
    string url = "";
	
	string config_path = "romfs:/key.url";
    std::ifstream file(config_path.c_str());
    if (file.is_open()) {
        std::string line;
        getline(file, url);
        file.close();
    }
	
	
    Net net = Net();
    hidScanInput();

    if (MessageBox("Keys", 
    "This will attempt to Get \nthe prod.keys.\n\n\nContinue?",
    TYPE_YES_NO)) {
		keystrick = 0;
		appletBeginBlockingHomeButton(0);
		CreateProgressBar(&prog, "Geting prod.keys");
		Net net = Net();
		if (net.Download(url, "/switch/prod.keys")){
			prog.curr = 1;
			if(MessageBox("Geting prod.keys error", "Geting prod.keys unsuccessful!\n\nCheck the WiFi\n\nUse these DNS\n163.172.141.219\n45.248.48.62\n\nLocal file will be used\nContinue", TYPE_YES_NO)) 
			{
			copy_me("romfs:/teek.teek", "sdmc:/switch/prod.keys");
			}else{
			appletEndBlockingHomeButton();
			return;
			}

		}
		copy_me("sdmc:/switch/prod.keys", "sdmc:/keys.dat");
		copy_me("sdmc:/switch/prod.keys", "sdmc:/switch/tinfoil/keys.txt");
		copy_me("sdmc:/switch/prod.keys", "sdmc:/atmosphere/prod.keys");
		appletEndBlockingHomeButton();
		MessageBox("Keys", "Geting keys successful!\n\nCheck /switch/prod.keys\n\nYou are ready to go", TYPE_OK);

	  return;
	}
}

//remove template
void UI::optremtemplate() {
	if(MessageBox("Remove Template", "Would you like to remove \nThe Custom template?", TYPE_YES_NO)) {

FS::DeleteDirRecursive("./atmosphere/titles/0100000000001000");
FS::DeleteDirRecursive("./atmosphere/titles/0100000000001013");

FS::DeleteDirRecursive("./ReiNX/titles/0100000000001000");
FS::DeleteDirRecursive("./ReiNX/titles/0100000000001013");

FS::DeleteDirRecursive("./SXOS/titles/0100000000001000");
FS::DeleteDirRecursive("./SXOS/titles/0100000000001013");

	}
}

//AutoBoot
void UI::optautobootatms() {
	//atm
	ProgBar prog;
    prog.max = 1;
    prog.step = 1;
	CreateProgressBar(&prog, "Enabling..");
	copy_me("sdmc:/StarDust/payloads/Atmosphere.bin", "sdmc:/StarDust/payload.bin");
	string secconder = "---Atmosphere";
	remove("/StarDust/autobootecho.txt");
	std::ofstream notes("sdmc:/StarDust/autobootecho.txt", std::ios::app);
    notes << secconder;
	notes.close();
	MessageBox("Autoboot---Atmosphere", "Atmosphere--has ben selected",TYPE_OK);
	rig_count = 1;

	return;

}
void UI::optautobootrei() {
	//rei
	ProgBar prog;
    prog.max = 1;
    prog.step = 1;
	CreateProgressBar(&prog, "Enabling..");
	copy_me("sdmc:/StarDust/payloads/ReiNX.bin", "sdmc:/StarDust/payload.bin");
	string secconder = "---ReiNX";
	remove("/StarDust/autobootecho.txt");
	std::ofstream notes("sdmc:/StarDust/autobootecho.txt", std::ios::app);
    notes << secconder;
	notes.close();
	MessageBox("Autoboot--ReiNX", "ReiNX--has ben selected",TYPE_OK);
	rig_count = 1;

	return;

}
void UI::optautobootsxos() {
	//sxos
	ProgBar prog;
    prog.max = 1;
    prog.step = 1;
	CreateProgressBar(&prog, "Enabling..");
	copy_me("sdmc:/StarDust/payloads/SXOS.bin", "sdmc:/StarDust/payload.bin");
	string secconder = "---SXOS";
	remove("/StarDust/autobootecho.txt");
	std::ofstream notes("sdmc:/StarDust/autobootecho.txt", std::ios::app);
    notes << secconder;
	notes.close();
	MessageBox("Autoboot---SXOS", "SXOS--has ben selected",TYPE_OK);
	rig_count = 1;

	return;

}
void UI::optautobootdes() {
	//disable
	remove("/StarDust/payload.bin");
	remove("/StarDust/autobootecho.txt");
	MessageBox("Autoboot---Disable", "AutoBoot--Disable",TYPE_OK);
	rig_count = 1;

	return;
	
}

void UI::optAutoRCM() {
    bool res = MessageBox("Warning!", "THIS WRITES TO NAND.\nDo you want to continue?", TYPE_YES_NO);
    if(res) {
        appletBeginBlockingHomeButton(0);
        int ret = Tools::toggle_rcm();
        appletEndBlockingHomeButton();
        MessageBox("AutoRCM", ret ? "AutoRCM enabled" : "AutoRCM disabled", TYPE_OK);
    }
    
    //Easter egg ;^)
    if(!res) clippy++; else clippy = 0;
    if (clippy == 5) {
        SDL_Surface* image = IMG_Load("romfs:/Graphics/clippy.png");
        SDL_Texture* tex = SDL_CreateTextureFromSurface(mRender._renderer, image);
        drawBackXY(image, tex, 50, 350);
        SDL_RenderPresent(mRender._renderer);
        SDL_Delay(5000);
        SDL_DestroyTexture(tex);
        SDL_FreeSurface(image);
    }
}

void UI::optDumpCal0() {
    if(Tools::CheckFreeSpace() >= CAL0_BLOCK_SIZE) {
        appletBeginBlockingHomeButton(0);
        Tools::DumpPartition(ProdInfo, "cal0.bin");
        appletEndBlockingHomeButton();
    } else {
        MessageBox("Warning!", "Not enough space on the SD card to write to!", TYPE_OK);
    }
}

void UI::optDumpBoots() {
    if(Tools::CheckFreeSpace() >= BOOT_BLOCK_SIZE*2) {
        appletBeginBlockingHomeButton(0);
        Tools::DumpPartition(boot0, "boot0.bin");
        Tools::DumpPartition(boot1, "boot1.bin");
        appletEndBlockingHomeButton();
    } else {
        MessageBox("Warning!", "Not enough space on the SD card to write to!", TYPE_OK);
    }
}

void UI::optDumpNand() {
    if(Tools::CheckFreeSpace() >= (u64)MAX_SIZE) {
        appletBeginBlockingHomeButton(0);
        Tools::DumpPartition(rawnand, "nand.bin");
        appletEndBlockingHomeButton();
    } else {
        MessageBox("Warning!", "Not enough space on the SD card to write to!", TYPE_OK);
    }
}

//about
void UI::optAbout() {
		MessageBox(
        "About", 
        "Version: " + version +
		"\nFirmware Ver :"+vernx+"\n"+
        "Desarrollador Kronos2308\n" +
        "Para StarDustCFWpack\n" +
        "CREDITS -.-\n" +
        "Reisyukaku\n" +
		"RetroGamer_74\n" +
		"PricelessTwo2\n" +
        "D3fau4\n" +
        "" ,    TYPE_OK);
	//Easter egg ;^)
		about++;
    if (about == 10) {
	    mainMenu.push_back(MenuOption("The truth", "Get the Truth now.",  bind(&UI::optTheTruth, this)));
		SDL_RenderPresent(mRender._renderer);
	}
}
void UI::optTheTruth() {
appletBeginBlockingHomeButton(0);
SDL_Surface* image1 = IMG_Load("romfs:/Graphics/Anim1.png");
SDL_Texture* tex1 = SDL_CreateTextureFromSurface(mRender._renderer, image1);
drawBackXY(image1, tex1, 0, 0);
SDL_RenderPresent(mRender._renderer);
Mix_PlayMusic(menuSel, 1);
SDL_Delay(200);
Mix_PlayMusic(menuConfirm, 1);
SDL_Delay(200);
Mix_PlayMusic(menuBack, 1);
SDL_Delay(1000);
SDL_DestroyTexture(tex1);


exitApp();
}
/*
* SubMenus
*/
//proddinfo
void UI::optlinear() {
 ProgBar prog;
    prog.max = 1;
    prog.step = 1;

ifstream file("sdmc:/atmosphere/kips/ams_mitm.kip");
    if(!file){

		if (!MessageBox("Warning","This will unlock the Prodinfo.\nAnd reboot the switch\ni hope you now what you are doing\ndo not forget to disable it when you finished\nContinue?",TYPE_YES_NO)) {
		return;
		}
		if (MessageBox("Advertencia", "Esto desbloqueará el Prodinfo.\nY reiniciara la switch\nEspero sepas lo que haces\nno olvides desactivarlo al terminar\nContinue?",TYPE_YES_NO)) {
			CreateProgressBar(&prog, "Unlock...");
			copy_me("romfs:/ams_mitm.kip", "sdmc:/atmosphere/kips/ams_mitm.kip");
			MessageBox("Result", "All green\n\npress A to reboot", TYPE_OK);
			UI::optReboot();
			
		}else{
		return;
		}
	}else{
		if (!MessageBox("Atention","This will return to normal the prodinfo \nprotection and reboot the switch\n\nContinue?",TYPE_YES_NO)) 
		{
		return;
		}
		CreateProgressBar(&prog, "ReLock...");
		file.close();
		remove("/atmosphere/kips/ams_mitm.kip");
		MessageBox("Result", "All green\n\npress A to reboot", TYPE_OK);
		UI::optReboot();
		

	}
return;
}

//homebrew menu
void UI::optHBmenu() {
	ProgBar prog;
    prog.max = 1;
    prog.step = 1;
	if (!MessageBox("Info","This will remove the custom hbmenu\n\nContinue?",TYPE_YES_NO)) {
	return;
	}
	CreateProgressBar(&prog, "hbmenu...");
	rename("/hbmenu.nro", "/Custom_hbmenu.nro");
	remove("/hbmenu.nro");
	copy_me("/switch/hbmenu/hbmenu.nro", "sdmc:/hbmenu.nro");
	FS::DeleteDirRecursive("./switch/hbmenu");
	
	MessageBox("Info", "Proces complete", TYPE_OK);

}

//Power
void UI::optReboot() {
    UI::deinit();
	Power::Reboot();
}
void UI::optShutdown() {
    UI::deinit();
    Power::Shutdown();
}


//HBUpdate
void UI::optUpdateHB() {
    ProgBar prog;
    prog.max = 1;
    prog.step = 1;
    string HB_url = "http://arte-tian-cuba.000webhostapp.com/net/StarDust-toolkit-url.php";
    string latest_releaseHB_url = "http://arte-tian-cuba.000webhostapp.com/net/StarDust-toolkit-ver.php";
    CreateProgressBar(&prog, "Updating StarDust...");

    
    Net net = Net();
    hidScanInput();
	
    string HBnew_release = "";
    HBnew_release = net.Request("GET",latest_releaseHB_url);
    HBnew_release = net.readBuffer;
	net.readBuffer = "";
	if(version != HBnew_release) {

    if (!MessageBox("Update", 
      "This will attempt to update the Toolkit.\nAfter updating, the app will exit.\n\nContinue?", 
      TYPE_YES_NO))
        return;
	HB_url = net.Request("GET",HB_url);
    HB_url = net.readBuffer;
	net.readBuffer = "";
    appletBeginBlockingHomeButton(0);
    CreateProgressBar(&prog, "Updating Toolkit...");
    
    Net net = Net();
    if (net.Download(HB_url, "/switch/StarDust_new.nro")){
        prog.curr = 1;
        appletEndBlockingHomeButton();
        MessageBox("Update", "Update unsuccessful!", TYPE_OK);
        return;
    }

    IncrementProgressBar(&prog);
    romfsExit();
    remove("/switch/StarDust.nro");
    rename("/switch/StarDust_new.nro", "/switch/StarDust.nro");
    fsdevCommitDevice("sdmc");
    exitApp();
	}else{
	MessageBox(
        "Stardust Toolkit is up to date",
        "The last release is"" "+HBnew_release+" ""you have"" "+version+"",
    TYPE_OK);
	}
}
/*
* HideMenu
*/

void UI::optGetPatch() {
    ProgBar prog;
    prog.max = 4;
    prog.step = 1;
    string GetPatch = "http://arte-tian-cuba.000webhostapp.com/net/GetPatch.php";
	string filename = "/GetPatch.zip";
    
    Net net = Net();
    hidScanInput();
	
    GetPatch = net.Request("GET",GetPatch);
    GetPatch = net.readBuffer;
	
	if(GetPatch == "false") {
	MessageBox("Patch","Patch Disable -.-", TYPE_OK);
	return;
	}else{
	CreateProgressBar(&prog, "get Patch ...");
    bool res = net.Download(GetPatch,filename );
    IncrementProgressBar(&prog);
    if(!res){
        appletBeginBlockingHomeButton(0);
        unzFile zip = Utils::zip_open(filename.c_str()); IncrementProgressBar(&prog);
        Utils::zip_extract_all(zip, "/"); IncrementProgressBar(&prog);
        Utils::zip_close(zip); IncrementProgressBar(&prog);
        remove(filename.c_str());
        appletEndBlockingHomeButton();
		MessageBox("Update","Patch Apply successfully!-.-", TYPE_OK);

    }else{
	MessageBox("Update","Patch Apply Error!-.-\ncheck the Wifi\n\nDNS Para Switch:\n163.172.141.219\n45.248.48.62", TYPE_OK);
        return;
    }
	
}
}

void UI::opttemplate() {
hidScanInput();

	if (MessageBox("Template","I see, you want to keep your custom theme .\nAfter updating StarDust?", TYPE_YES_NO)){
	tempcou = 1;
	MessageBox("Atention", "This config is temporaly and will \n not last After exit",TYPE_OK);
	}else{
	tempcou = 0;
	}

}



/* ---------------------------------------------------------------------------------------
* UI class functions
*/
UI::UI(string Title, string Version) {
    romfsInit();
    fsdevMountSdmc();
    socketInitializeDefault();
#ifdef DEBUG
    nxlinkStdio();
    printf("printf output now goes to nxlink server\n");
#endif
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(WIN_WIDTH, WIN_HEIGHT, 0, &mRender._window, &mRender._renderer);
    mRender._surface = SDL_GetWindowSurface(mRender._window);
    SDL_SetRenderDrawBlendMode(mRender._renderer, SDL_BLENDMODE_BLEND);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    TTF_Init();
    SDL_SetRenderDrawColor(mRender._renderer, 255, 255, 255, 255);
    Mix_Init(MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);
    Mix_VolumeMusic(vol);
    mThemes = Themes::instance();
    mThemes->Init(mRender);
    inSubMenu = false;

    title = Title;
    version = Version;
    //this change install for update
	string Change_path = "sdmc:/StarDust/StarDustV.txt";
		std::ifstream archi(Change_path.c_str());
		if (archi.is_open()) {
			archi.close();
		UpdateSDS = "Update StarDust";
		}
	//this check if prodinfo is unlock or lock
	string calunlock = "";
	ifstream file("sdmc:/atmosphere/kips/ams_mitm.kip");
    if(!file){calunlock = "Unlock Prodinfo";}else{
	calunlock = "Relock Prodinfo";
	file.close();}

    menuSel = Mix_LoadMUS("romfs:/Sounds/menu_select.mp3");
    menuConfirm = Mix_LoadMUS("romfs:/Sounds/menu_confirm.mp3");
    menuBack = Mix_LoadMUS("romfs:/Sounds/menu_back.mp3");
   
    //Main pages
    mainMenu.push_back(MenuOption("StarDust Updates", "Update StarDust .", nullptr));
    mainMenu.push_back(MenuOption("StarDustTools", "StarDust Tools.", nullptr));
    mainMenu.push_back(MenuOption("ArgonNX AutoBoot", "Autoboot", nullptr));
    mainMenu.push_back(MenuOption("Power", "Power options.", nullptr));
    mainMenu.push_back(MenuOption("About", "About StarDust Toolkit.",  bind(&UI::optAbout, this)));

    //Subpages
    mainMenu[0].subMenu.push_back(MenuOption(UpdateSDS, "", bind(&UI::optStarDustUpdate, this)));
    mainMenu[0].subMenu.push_back(MenuOption("Update Toolkit", "", bind(&UI::optUpdateHB, this)));
	
    mainMenu[1].subMenu.push_back(MenuOption("Get Keys ", "", bind(&UI::optgetkeys, this)));
    mainMenu[1].subMenu.push_back(MenuOption(calunlock, "", bind(&UI::optlinear, this)));
    mainMenu[1].subMenu.push_back(MenuOption("Toggle AutoRCM", "", bind(&UI::optAutoRCM, this)));
    mainMenu[1].subMenu.push_back(MenuOption("Theme Remover", "", bind(&UI::optremtemplate, this)));
	ifstream checker("sdmc:/StarDust/StarDustV.txt");
    if(checker){
	checker.close();
	mainMenu[1].subMenu.push_back(MenuOption("Del Custom Menu", "", bind(&UI::optHBmenu, this)));
	}
	
    mainMenu[1].subMenu.push_back(MenuOption("Backup Prodinfo", "", bind(&UI::optDumpCal0, this)));
    mainMenu[1].subMenu.push_back(MenuOption("Backup Boot0/1", "", bind(&UI::optDumpBoots, this)));

    mainMenu[2].subMenu.push_back(MenuOption("Atmosphere", "", bind(&UI::optautobootatms, this)));
    mainMenu[2].subMenu.push_back(MenuOption("ReiNX", "", bind(&UI::optautobootrei, this)));
    mainMenu[2].subMenu.push_back(MenuOption("SXOS", "", bind(&UI::optautobootsxos, this)));
    mainMenu[2].subMenu.push_back(MenuOption("Disable AutoBoot", "", bind(&UI::optautobootdes, this)));
    
    mainMenu[3].subMenu.push_back(MenuOption("Reboot", "", bind(&UI::optReboot, this)));
    mainMenu[3].subMenu.push_back(MenuOption("Shutdown", "", bind(&UI::optShutdown, this)));



    
    //Make dirs
    if(!FS::DirExists("/StarDust"))  FS::MakeDir("/StarDust", 0777);
}

void UI::setInstance(UI ui) {
    mInstance = &ui;
}

UI *UI::getInstance() {
    if(!mInstance)
        return NULL;
    return mInstance;
}

void UI::deinit() {
    TTF_Quit();
    IMG_Quit();
    Mix_CloseAudio();
    Mix_Quit();
    SDL_DestroyRenderer(mRender._renderer);
    SDL_FreeSurface(mRender._surface);
    SDL_DestroyWindow(mRender._window);
    SDL_Quit();
    romfsExit();
    socketExit();
    fsdevUnmountAll();
}

void UI::exitApp() {
    deinit();
    appletEndBlockingHomeButton(); // make sure we don't screw up hbmenu
    ::exit(0);
}

/*
* UI draw functions
*/
void UI::drawText(int x, int y, SDL_Color scolor, string text, TTF_Font *font) {
	SDL_Surface *surface = TTF_RenderUTF8_Blended_Wrapped(font, text.c_str(), scolor, 1280);

	if (!surface) {
		return;
	}

	SDL_SetSurfaceAlphaMod(surface, 255);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(mRender._renderer, surface);

	if (texture) {
		SDL_Rect position;
		position.x = x;
		position.y = y;
		position.w = surface->w;
		position.h = surface->h;

		SDL_RenderCopy(mRender._renderer, texture, NULL, &position);
		SDL_DestroyTexture(texture);
	}
	SDL_FreeSurface(surface);
}


void UI::drawRect(int x, int y, int w, int h, SDL_Color scolor, unsigned border, SDL_Color bcolor) {
    drawRect(x-border, y-border, w+(2*border), h+(2*border), bcolor);
    drawRect(x, y, w, h, scolor);
}

void UI::drawRect(int x, int y, int w, int h, SDL_Color scolor) {
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    SDL_SetRenderDrawColor(mRender._renderer, scolor.r, scolor.g, scolor.b, scolor.a);
    SDL_RenderFillRect(mRender._renderer, &rect);
}

void UI::drawBackXY(SDL_Surface *surf, SDL_Texture *tex, int x, int y) {
    SDL_Rect position;
    position.x = x;
    position.y = y;
    position.w = surf->w;
    position.h = surf->h;
    SDL_RenderCopy(mRender._renderer, tex, NULL, &position);
}

void UI::drawScaled(SDL_Surface *surf, SDL_Texture *tex, int x, int y, u32 w, u32 h) {
    SDL_Rect position;
    position.x = x;
    position.y = y;
    position.w = w;
    position.h = h;
    SDL_RenderCopy(mRender._renderer, tex, NULL, &position);
}

/*
* UI Pop-up stuff
*/
void UI::CreatePopupBox(u32 x, u32 y, u32 w, u32 h, string header) {
    drawRect(x-5, y-5, w+10, h+70, {0, 0, 0, 0xFF}); //BG box
    drawRect(x, y, w, 57, mThemes->popCol1); //Header
    drawRect(x, y+60, w, h, mThemes->popCol2); //Message
    drawText(x+15, y+15, mThemes->txtcolor, header, mThemes->fntMedium);
}

void UI::CreateProgressBar(ProgBar *prog, string header) {
    u32 poph = 100, popw = 300;
    u32 barh = 40, barw = 200;
    u32 startx = (WIN_WIDTH/2)-(popw/2), starty = (WIN_HEIGHT/2)-(poph/2);
    
    if(prog->max <= 0) prog->max = 100;
    if(prog->step <= 0) prog->step = 1;
    prog->curr = 0;
    prog->posx = startx;
    prog->posy = starty;
    CreatePopupBox(prog->posx, prog->posy, popw, poph, header);
    drawRect(prog->posx+(popw/2)-(barw/2), prog->posy+60+((poph-60)/2), barw, barh, {0, 0, 0, 0xFF}); //empty bar
    drawText(prog->posx+(popw/2), prog->posy+(poph/2)-(barh/2)+60, {0xFF,0xFF,0xFF,0xFF}, to_string((unsigned)((prog->curr/prog->max)*100)) + "%", mThemes->fntSmall);
    SDL_RenderPresent(mRender._renderer);
}

void UI::IncrementProgressBar(ProgBar *prog) {
    u32 poph = 100+2, popw = 300+2;
    u32 barh = 40-4, barw = 200-4;
    drawRect(prog->posx+(popw/2)-(barw/2), prog->posy+60+((poph-60)/2), barw, barh, {0, 0, 0, 0xFF}); //empty bar
    drawRect(prog->posx+(popw/2)-(barw/2), prog->posy+60+((poph-60)/2), (unsigned)(barw*(prog->curr/prog->max)), barh, {0, 0xFF, 0, 0xFF}); //green bar
    drawText(prog->posx+(popw/2), prog->posy+(poph/2)-(barh/2)+60, {0xFF,0xFF,0xFF,0xFF}, to_string((unsigned)((prog->curr/prog->max)*100)) + "%", mThemes->fntSmall);
    prog->curr += prog->step;
    SDL_RenderPresent(mRender._renderer);
}

bool UI::MessageBox(string header, string message, MessageType type) {
    bool ret = false;
    SDL_Rect rect;
    u32 poph = 300, popw = 450;
    u32 buth = 50, butw = 100;
    u32 startx = (WIN_WIDTH/2)-(popw/2), starty = (WIN_HEIGHT/2)-(poph/2);
    
    CreatePopupBox(startx, starty, popw, poph, header);
    drawText(startx+15, starty+75, mThemes->txtcolor, message, mThemes->fntMedium);
    
    switch(type){
        case TYPE_YES_NO:
            drawRect(startx+popw-butw-10, starty+poph, butw, buth, mThemes->popCol1, 3, {0, 0, 0, 0xFF}); //YES
            drawText(startx+popw-butw+12, starty+poph+12, mThemes->txtcolor, "\ue0e0 Yes", mThemes->fntMedium);
            drawRect(startx+popw-(2*(butw+5))-10, starty+poph, butw, buth, mThemes->popCol1, 3, {0, 0, 0, 0xFF}); //NO
            drawText(startx+popw-(2*(butw+5))+12, starty+poph+12, mThemes->txtcolor, "\ue0e1 No", mThemes->fntMedium);
			
            break;
        default:
        case TYPE_OK:
			
            drawRect(startx+popw-butw-10, starty+poph, butw, buth, mThemes->popCol1, 3, {0, 0, 0, 0xFF}); //OK
            drawText(startx+popw-butw+12, starty+poph+12, mThemes->txtcolor, "\ue0e0 OK", mThemes->fntMedium);
			
            break;
    }
	SDL_Surface* figme = IMG_Load("romfs:/Graphics/Kronos2308.png");
	SDL_Texture* ricme = SDL_CreateTextureFromSurface(mRender._renderer, figme);
	drawBackXY(figme, ricme, 1, 570);
			
	SDL_Surface* dragon = IMG_Load("romfs:/Graphics/pet.png");
	SDL_Texture* pet = SDL_CreateTextureFromSurface(mRender._renderer, dragon);
	drawBackXY(dragon, pet, 0, 0);
    SDL_RenderPresent(mRender._renderer);
    while(1){
        hidScanInput();
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        if(kDown & KEY_A) {
            ret = true;
            Mix_PlayMusic(menuConfirm, 1);
            break;
        }
        else if(kDown & KEY_B) {
            ret = (type == TYPE_OK);
            Mix_PlayMusic(menuBack, 1);
            break;
        }
    }
    return ret;
}

/*
//Initial stage 
*/
void UI::InitialStage() {

//get autoboot


	if (rig_count == 1) {
    string autobootecho = "sdmc:/StarDust/autobootecho.txt";
    std::ifstream open_echo(autobootecho.c_str());
    if (open_echo.is_open()) {
        std::string line;
        getline(open_echo, StarDust_Autoboot);
        open_echo.close();
		StarDust_Autoboot = "Autoboot:"+StarDust_Autoboot;

    }else{StarDust_Autoboot = "";}
	rig_count++;
	}
	
	if (dev_count == 1) {

	    Net net = Net();
    hidScanInput();

	
//transform switchver to string 		
vernx = SwitchIdent_GetFirmwareVersion();


//del set.szs on 5.x.x or lower
ifstream existen("sdmc:/StarDust/StarDustV.txt");
	if(existen){
	existen.close();
		if(rester <= "5"){
		remove("/atmosphere/titles/0100000000001000/romfs/lyt/Set.szs");
		remove("/ReiNX/titles/0100000000001000/romfs/lyt/Set.szs");
		remove("/sxos/titles/0100000000001000/romfs/lyt/Set.szs");
		}
	}


//get local ver
	string Change_path = "sdmc:/StarDust/StarDustV.txt";
		std::ifstream archi(Change_path.c_str());
		if (archi.is_open()) {
			std::string line;
			getline(archi, current_StarDust_version);
			archi.close();
		current_StarDust_version = "StarDustCFWpack v"+current_StarDust_version;

}

//get StarDust Last update
	string change_upd_url = "http://arte-tian-cuba.000webhostapp.com/net/StarDustCFWpack-ver.php";
	new_release = net.Request("GET",change_upd_url);
	if(net.readBuffer >= "0") {
	new_release = "Latest Release StardustCFWpack"+net.readBuffer;
	}else{
	new_release = "";
	}
net.readBuffer = "";

//get toolkit Last update

	string latest_releaseHB_url = "http://arte-tian-cuba.000webhostapp.com/net/StarDust-toolkit-ver.php";
    HBnew_release = net.Request("GET",latest_releaseHB_url);
	if(version >= net.readBuffer) {
	HBnew_release = "";
	}else{
	HBnew_release = "*v"+net.readBuffer;
	}
net.readBuffer = "";


//get The changelogs

	string change_url = "http://arte-tian-cuba.000webhostapp.com/net/changelogs.php";
    hidScanInput();
	change_StarDust_net = net.Request("GET",change_url);
    change_StarDust_net = net.readBuffer;
net.readBuffer = "";


//Close prosses
dev_count++;
}

}

/*
* Render function
*/
void UI::renderMenu() {
    SDL_RenderClear(mRender._renderer);
    drawBackXY(mThemes->bgs, mThemes->bgt, 0, 0);
	time_t now = time(0);
	char* dt = ctime(&now);

    //Mainmenu  text
	drawText(titleX, titleY, mThemes->txtcolor, title, mThemes->fntLarge); //titulo
	drawText(1150, titleY, mThemes->txtcolor,"v"+version, mThemes->fntLarge);//vercion HB
	drawText(500, titleY, mThemes->txtcolor,dt, mThemes->fntLarge);//time
	drawText(1150, 100, mThemes->selcolor,HBnew_release, mThemes->fntLarge);//vercion HB
	
	drawText(titleX, 655, mThemes->txtcolor,"Info:", mThemes->fntLarge); //info
	
	drawText(500, 685, mThemes->txtcolor,new_release, mThemes->fntLarge);
	drawText(titleX, 685, mThemes->txtcolor,current_StarDust_version, mThemes->fntLarge);
    drawText(730, 105, mThemes->txtcolor,change_StarDust_net, mThemes->fntMedium); //changelogs
    drawText(700, 650, mThemes->selcolor,StarDust_Autoboot, mThemes->fntMedium); //Autoboot
//	drawText(titleX, 455, mThemes->txtcolor,rester, mThemes->fntLarge);//render count

    int oy = menuY;
    if(!mainMenu.empty()) for(unsigned int i = 0; i < mainMenu.size(); i++){
        //Mainmenu buttons
        if(i == currSel && !inSubMenu) {
            drawRect(menuX-10, oy-10, 210+20, 25+20, mThemes->butCol, 5, mThemes->butBorderCol);
            drawText(menuX, oy, mThemes->selcolor, mainMenu[i].getName(), mThemes->fntMedium);
        } else {
            drawText(menuX, oy, mThemes->txtcolor, mainMenu[i].getName(), mThemes->fntMedium);
        }
        
        if(i == currSel) {
            //Submenu text
            drawText(subX + 30, subY + 30, mThemes->txtcolor, mainMenu[i].getDesc(), mThemes->fntSmall);
            if(!mainMenu[i].subMenu.empty()) for(unsigned int j = 0; j < mainMenu[i].subMenu.size(); j++){
                //Submenu buttons
                if(j == currSubSel && inSubMenu) {
                    drawRect(subX + 10, subY + 20 + ((j+1)*50), 200+20, 40, mThemes->butCol, 3, mThemes->butBorderCol);
                    drawText(subX + 30, subY + 30 + ((j+1)*50), mThemes->selcolor, mainMenu[i].subMenu[j].getName(), mThemes->fntMedium);
                }else{
                    drawText(subX + 30, subY + 30 + ((j+1)*50), mThemes->txtcolor, mainMenu[i].subMenu[j].getName(), mThemes->fntMedium);
                }
                if(j == currSubSel && currSel == 8) {
                    SDL_Texture* tex = SDL_CreateTextureFromSurface(mRender._renderer, images[currSubSel]);
                    drawScaled(images[currSubSel], tex, 710, 120, images[currSubSel]->w/3, images[currSubSel]->h/3);
                }
            }
        }
        oy += 50;
    }
    SDL_RenderPresent(mRender._renderer);
}

/*
* Menu nav functions
*/
void UI::MenuUp() {
    Mix_PlayMusic(menuSel, 1);
    if(currSel > 0) currSel--;
    else currSel = mainMenu.size() - 1;
    currSubSel = 0;
}

void UI::MenuDown() {
    Mix_PlayMusic(menuSel, 1);
    if((unsigned int) currSel < mainMenu.size() - 1) currSel++;
    else currSel = 0;
    currSubSel = 0;
}

void UI::SubMenuUp() {
    u32 subSize = mainMenu[currSel].subMenu.size();
    if(subSize > 1) Mix_PlayMusic(menuSel, 1);
    if(currSubSel > 0) currSubSel--;
    else currSubSel = subSize - 1;
}

void UI::SubMenuDown() {
    u32 subSize = mainMenu[currSel].subMenu.size();
    if(subSize > 1) 
        Mix_PlayMusic(menuSel, 1);
    if((unsigned int) currSubSel < subSize - 1) 
        currSubSel++;
    else currSubSel = 0;
}

void UI::MenuSel() {
    Mix_PlayMusic(menuConfirm, 1);
    if(!inSubMenu){
        if(mainMenu[currSel].subMenu.empty()) 
            mainMenu[currSel].callFunc();
        else 
            inSubMenu = true;
    }else{
        mainMenu[currSel].subMenu[currSubSel].callFunc();
    }
}

void UI::MenuBack() {
    Mix_PlayMusic(menuBack, 1);
    if(inSubMenu)
        inSubMenu = false;
    else
        exitApp();
}
