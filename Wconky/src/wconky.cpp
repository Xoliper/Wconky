#include <iostream>
#include <exception>
#include <string>
#include <windows.h>


//TODO list:
//Disk personalization
//Error handling!

using namespace std;

#define GLFW_DLL
#include "resource.h"

//Include Core Engine
#include "Engine/Common/config.hpp"
#include "Engine/Wrappers/opengl.hpp"
#include "Engine/Modules/timer.hpp"
#include "Engine/Common/shaderloader.hpp"

//Include Modules
#include "Engine/Common/texture.hpp"
#include "Engine/Common/sprite.hpp"
#include "Engine/Common/visualobject.hpp"
#include "Engine/Common/world.hpp"
#include "Engine/Common/font.hpp"
#include "Engine/Common/text.hpp"
#include "Engine/Common/primitive.hpp"

//Experimental
#include "Engine/Common/Exceptions.hpp"
#include "Engine/Common/XmlParser.hpp"
#include "Engine/Common/Color.hpp"
#include "Engine/Extras/MonitorInfo.hpp"


//Wconky modules
#include "theme.hpp"
#include "graph.hpp"
#include "textcnt.hpp"
#include "cpu.hpp"
#include "ram.hpp"
#include "time.hpp"
#include "Lang.hpp"
#include "Disk.hpp"

//Wconky Extensions
#include "Extensions/manage-icons-ext.h"



#define ID_TRAY1   601
#define CMSG_TRAY1 0x8001
#define IDM_ABOUT 0x8005
#define IDM_RELOAD 0x8004
#define IDM_EDITOR 0x8010
#define IDM_EXIT 0x8002
#define IDM_SEP 0x8003

HMENU hPopMenu;
bool reload = true;
bool exitflag = false;

static WNDPROC currentWndProc;
static WNDPROC WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
switch(umessage)
{

	case WM_COMMAND: {
		if(IDM_EXIT == LOWORD(wparam)){
			reload = false;
			exitflag = true;
		} else if(IDM_RELOAD == LOWORD(wparam)){
			reload = true;
			std::cout<<"Reloading"<<std::endl;
		} else if(IDM_ABOUT == LOWORD(wparam)){
		    MessageBox( NULL, "The creator of Wconky is Arkadiusz Jarlaczyk (xoliper), programmer of \"Arpeji Studio\". Contact e-mail: arkadiusz@jarlaczyk.pl", "   About Wcony", MB_ICONINFORMATION | MB_OK );
			SetForegroundWindow((HWND)GetDesktopWindow());
		} else if(IDM_EDITOR == LOWORD(wparam)){
			system ("start \"\" \"wconkyTE.exe\"");
		}

	} break;

	case CMSG_TRAY1:
	{
		if( wparam == ID_TRAY1 ){
			if( lparam == WM_RBUTTONDOWN ){
				POINT lpClickPoint;
				GetCursorPos(&lpClickPoint);
				SetForegroundWindow(hwnd);
				TrackPopupMenu(hPopMenu,TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_TOPALIGN, lpClickPoint.x, lpClickPoint.y,0,hwnd,NULL);
			}
		}
	} break;

    // All other messages pass to the message handler in the system class.
    default:
    {
        return (WNDPROC)CallWindowProc(currentWndProc, hwnd, umessage, wparam, lparam);
    }
}
return 0;
}




int main(int argc, char* argv[]) {

	bool reloaded = false;

	while(reload and !exitflag){

	std::cout<<"Init"<<std::endl;

	//Config id should be stored in argv[0]
	int config_id = 0;

	//Load configuration file
	XmlParser config;
	config.load("config.xml", true);
	config.parse();
	//config.printTree();

	std::list<Node*> allConfigs = config.getNodes({"configurations", "config"});
	Node * targetConfig = nullptr;
	if(config.checkResult(&allConfigs)){
		for(auto & it : allConfigs){
			if((*it).getAttr<int>("id") == config_id){
				targetConfig = it;
				break;
			}
		}

		//--------------------------------------------------------------------------------------------------------
		//Main program part
		//--------------------------------------------------------------------------------------------------------
		if(targetConfig != nullptr){

			//Get Delay value
			int delay = 0;
			std::list<Node*> delayNode = config.getNodes({"settings", "delay"}, targetConfig);
			if(delayNode.size() == 1 and delayNode.front() != nullptr){
				delay = delayNode.front()->getValue<int>();
			}

			//--------------------------------------------------------------------------------------------------------
			//Get monitor data
			//--------------------------------------------------------------------------------------------------------
			int selMonit = -1;
			std::list<Node*> selectedMonitor = config.getNodes({"settings", "monitor"}, targetConfig);
			if(selectedMonitor.size() == 1 and selectedMonitor.front() != nullptr){
				selMonit = selectedMonitor.front()->getValue<int>();
			} else {
				std::cout<<"Exce 3"<<std::endl;
			    throw Exception("File", "config.xml", "Cannot find monitor config...");
				return 3;
			}
			RECT monRect;
			MonitorInfo mi(selMonit);
			if(mi.MonitorsAmount() >= selMonit){
				monRect = mi.GetMonitorRect();
			}

			//--------------------------------------------------------------------------------------------------------
			//Load lang data
			//--------------------------------------------------------------------------------------------------------
			Lang lang;
			Node* langNode = config.getNodes({"lang", "days"}, targetConfig).front();
			if(langNode != nullptr){
				lang.days[0] = langNode->getAttr<std::string>("monday");
				lang.days[1] = langNode->getAttr<std::string>("tuesday");
				lang.days[2] = langNode->getAttr<std::string>("wednesday");
				lang.days[3] = langNode->getAttr<std::string>("thursday");
				lang.days[4] = langNode->getAttr<std::string>("friday");
				lang.days[5] = langNode->getAttr<std::string>("saturday");
				lang.days[6] = langNode->getAttr<std::string>("sunday");
				lang.daysInit = true;
			}
			langNode = config.getNodes({"lang", "months"}, targetConfig).front();
			if(langNode != nullptr){
				lang.months[0] = langNode->getAttr<std::string>("jan");
				lang.months[1] = langNode->getAttr<std::string>("feb");
				lang.months[2] = langNode->getAttr<std::string>("mar");
				lang.months[3] = langNode->getAttr<std::string>("apr");
				lang.months[4] = langNode->getAttr<std::string>("may");
				lang.months[5] = langNode->getAttr<std::string>("june");
				lang.months[6] = langNode->getAttr<std::string>("july");
				lang.months[7] = langNode->getAttr<std::string>("aug");
				lang.months[8] = langNode->getAttr<std::string>("sept");
				lang.months[9] = langNode->getAttr<std::string>("oct");
				lang.months[10] = langNode->getAttr<std::string>("nov");
				lang.months[11] = langNode->getAttr<std::string>("dec");
				lang.monthsInit = true;
			}

			//--------------------------------------------------------------------------------------------------------
			//Load extensions
			//--------------------------------------------------------------------------------------------------------

			//ManageIcons Ext
			ManageIcons * manageIcons = nullptr;
			int manageIconsKey = 0;
			Node* ManageIconsNode = config.getNodes({"Extensions", "ManageIcons"}, targetConfig).front();
			if(ManageIconsNode != nullptr){
				manageIcons = new ManageIcons();
				manageIconsKey = std::stoi(ManageIconsNode->getAttr<std::string>("hotkey"), nullptr, 16);
			}

			//--------------------------------------------------------------------------------------------------------
			//Initialize engine
			//--------------------------------------------------------------------------------------------------------
			OpenglManager ogl(selMonit);
			ogl.CreateGLWindow("wconky", 90.0f, 100,100);//monRect.right-monRect.left, monRect.bottom-monRect.top);
			//glfwSetWindowSize(ogl.GetHandle(),1920,1080);
			ogl.SetupWindow();
			ogl.SetVSync(!FPS_DEBUG);

			World & world = *ogl.GetWorldHandle();
			world.Move(1.0f, 1.0f,0.0f);

			ShaderLoader sl;
			//sl.Load("src/Engine/Shaders/shaders_indexer.txt");
			sl.Load("Shaders/shaders_indexer.txt");


			//--------------------------------------------------------------------------------------------------------
			//Winapi & Hacking procedure
			//--------------------------------------------------------------------------------------------------------



			//Get our hwnd
			HWND hwnd = ogl.GetHWND();

			//Add systray
			PSTR sTip = "WConky";
			LPSTR sTytul = "WConky Message";
			LPSTR sOpis = "Hey, i am accessible via Tray icon!";
			LPSTR sOpis2 = "Configuration file reloaded!";

			NOTIFYICONDATA nid;
			nid.cbSize = sizeof( NOTIFYICONDATA );
			nid.hWnd = hwnd;
			nid.uID = ID_TRAY1;
			nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_INFO;
			nid.uCallbackMessage = CMSG_TRAY1;
			nid.hIcon = LoadIcon( GetModuleHandle(NULL), MAKEINTRESOURCE(GLFW_ICON));
			nid.dwInfoFlags = NIIF_INFO; // ikonka ostrzeżenie
			lstrcpy( nid.szTip, sTip );
			lstrcpy( nid.szInfoTitle, sTytul );
			if(reloaded == false) {
				lstrcpy( nid.szInfo, sOpis );
			} else {
				lstrcpy( nid.szInfo, sOpis2 );
			}
			Shell_NotifyIcon(NIM_ADD, &nid);

			hPopMenu = CreatePopupMenu();
			InsertMenu(hPopMenu,0xFFFFFFFF,MF_BYCOMMAND|MF_STRING,IDM_RELOAD,("Reload configuration"));
			InsertMenu(hPopMenu,0xFFFFFFFF,MF_BYCOMMAND|MF_STRING,IDM_EDITOR,("Edit theme"));

			InsertMenu(hPopMenu,0xFFFFFFFF,MF_SEPARATOR,IDM_SEP,("Separator"));
			InsertMenu(hPopMenu,0xFFFFFFFF,MF_BYCOMMAND|MF_STRING,IDM_ABOUT,("About"));
			InsertMenu(hPopMenu,0xFFFFFFFF,MF_BYCOMMAND|MF_STRING,IDM_EXIT,("Exit"));

			//Bypass default WndProc
			currentWndProc = (WNDPROC)GetWindowLongPtr(hwnd, GWL_WNDPROC);
			SetWindowLongPtr(hwnd, GWL_WNDPROC, (long)WndProc);

			//Make it shown after background
			HWND progman = FindWindowEx(NULL, NULL, TEXT("Progman"), NULL);
			SendMessage(progman, 0x052C, 0,0);
			//Get HWND to that WorkrW
			HWND hDefView = FindWindowEx(NULL, NULL, NULL, TEXT("Program Manager"));
			HWND worker = GetNextWindow(hDefView, GW_HWNDPREV);
			HWND worker2 = GetNextWindow(worker, GW_HWNDPREV);
			SetParent(hwnd,worker);
			SetWindowPos(hwnd, worker,  monRect.left - mi.GetMinX(), monRect.top - mi.GetMinY(), monRect.right-monRect.left, monRect.bottom-monRect.top, SWP_FRAMECHANGED  | SWP_NOZORDER | SWP_NOOWNERZORDER);


			//Change window style
			//LONG lStyle = GetWindowLong(hwnd, GWL_STYLE);
			//lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZE | WS_MAXIMIZE | WS_SYSMENU);
			//SetWindowLong(hwnd, GWL_STYLE, lStyle);
			//LONG lExStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
			//lExStyle &= ~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
			//SetWindowLong(hwnd, GWL_EXSTYLE, lExStyle);
			//SetWindowPos(hwnd, NULL, 0,0,0,0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER);


			/*LONG lStyle = GetWindowLong(hwnd, GWL_STYLE);
			lStyle |= (WS_CAPTION | WS_THICKFRAME | WS_MINIMIZE | WS_MAXIMIZE | WS_SYSMENU);
			SetWindowLong(hwnd, GWL_STYLE, lStyle);
			LONG lExStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
			lExStyle |= (WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
			SetWindowLong(hwnd, GWL_EXSTYLE, lExStyle);
			SetWindowPos(hwnd, NULL, 0,0,0,0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER);
			*/


			//Remove from taskbar
			ShowWindow(hwnd, SW_HIDE);
			SetWindowLongPtr(hwnd, GWL_EXSTYLE,GetWindowLongPtr(hwnd, GWL_EXSTYLE) | WS_EX_TOOLWINDOW);
			ShowWindow(hwnd, SW_SHOW);

			//Fix window size
			glfwSetWindowSize(ogl.GetHandle(),monRect.right-monRect.left, monRect.bottom-monRect.top);
			world.SetupFov(90.0f, monRect.right-monRect.left, monRect.bottom-monRect.top);
   			glViewport(0, 0, monRect.right-monRect.left, monRect.bottom-monRect.top);

   			//Fix win10 fullscreen mode
   			//No fix???

			SetForegroundWindow((HWND)GetDesktopWindow());

			//--------------------------------------------------------------------------------------------------------
			//Load theme
			//--------------------------------------------------------------------------------------------------------
			std::vector<Theme*> themes;
			std::list<Node*> allThemes = config.getNodes({"theme"}, targetConfig);
			for(auto & th : allThemes){

				//Font file, size, color

				std::cout<<"Theme: "<<th->getAttr<int>("id")<<std::endl;

				std::list<Node*> temp_c_font = config.getNodes({"font"}, th);
				std::string temp_color;
				std::cout<<"Font list size:"<<temp_c_font.size()<<std::endl;
				if(temp_c_font.front() != nullptr) temp_color = temp_c_font.front()->getAttr<std::string>("color");

				//Other colors
				std::string graphBars_s;
				int graphBars = 0;
				std::string graphBase_s;
				int graphBase = 0;
				std::list<Node*> graph_th = config.getNodes({"graphTheme"}, th);
				if(graph_th.front() != nullptr){
					graphBars_s = graph_th.front()->getAttr<std::string>("graphBarsCol");
					graphBars = std::stoi(graphBars_s, nullptr, 16);
					graphBase_s = graph_th.front()->getAttr<std::string>("graphBaseCol");
					graphBase = std::stoi(graphBase_s, nullptr, 16);
				}

				Theme * theme = new Theme;
				//Check if font already loaded
				std::string fntPathTemp;
				if(temp_c_font.front() != nullptr) fntPathTemp = temp_c_font.front()->getValue<std::string>();
				int fntSizeTemp;
				if(temp_c_font.front() != nullptr) fntSizeTemp = temp_c_font.front()->getAttr<int>("size");

				for(auto & th : themes){
					//Check by font if available
					if(temp_c_font.front() != nullptr and th->fnt != nullptr){
						if(th->fnt->path == fntPathTemp and th->fnt->size == fntSizeTemp){
							theme->fnt = th->fnt;
						}
					}
				}
				if(temp_c_font.front() != nullptr){
					if(theme->fnt == nullptr){
						theme->fnt = new Font(fntPathTemp, fntSizeTemp);
					}
				}
				if(temp_c_font.front() != nullptr) theme->color = Color(std::stoi(temp_color, nullptr, 16));
				if(temp_c_font.front() != nullptr) theme->color.SetOpacity(temp_c_font.front()->getAttr<int>("opacity"));
				if(graph_th.front() != nullptr){
					theme->graphBars = Color(graphBars);
					theme->graphBars.SetOpacity(graph_th.front()->getAttr<int>("opacity"));
					theme->graphBase = Color(graphBase);
					theme->graphBase.SetOpacity(graph_th.front()->getAttr<int>("opacity"));
					theme->graphBarWidth = graph_th.front()->getAttr<int>("barWidth");
					theme->graphBarSpacing = graph_th.front()->getAttr<int>("barSpacing");
				}

				Node* disk_th = config.getNodes({"diskTheme"}, th).front();
				if(disk_th != nullptr){
					int opacity = 255;
					int temp_op = disk_th->getAttr<int>("opacity");
					if(disk_th->ifGood()){
						opacity = temp_op;
					}


					theme->diskBorderCol = Color(std::stoi(disk_th->getAttr<std::string>("borderColor"), nullptr, 16));
					theme->diskBorderCol.SetOpacity(opacity);
					theme->diskFillCol = Color(std::stoi(disk_th->getAttr<std::string>("fillColor"), nullptr, 16));
					theme->diskFillCol.SetOpacity(opacity);
					theme->diskBorderThickness = disk_th->getAttr<int>("borderThickness");
					theme->diskBorderHeight = disk_th->getAttr<int>("borderHeight");

					theme->diskFillCol.Print();


				}

				themes.push_back(theme);

			}

			//--------------------------------------------------------------------------------------------------------
			//Check modules config & load modules
			//--------------------------------------------------------------------------------------------------------

			Cpu * cpu = nullptr;
			Ram * ram = nullptr;
			Disk * disk = nullptr;
			TimeCnt * time = nullptr;

			std::list<Node*> moduleNode;
			moduleNode = config.getNodes({"modules", "cpu"}, targetConfig);
			if(moduleNode.size() == 1 and moduleNode.front() != nullptr){
				bool cpu_m = moduleNode.front()->getValue<bool>();
				if(cpu_m){

					cpu = new Cpu(moduleNode.front()->getAttr<int>("fps"));
				}
			}
			moduleNode = config.getNodes({"modules", "ram"}, targetConfig);
			if(moduleNode.size() == 1 and moduleNode.front() != nullptr){
				bool ram_m = moduleNode.front()->getValue<bool>();
				if(ram_m){
					ram = new Ram(moduleNode.front()->getAttr<int>("fps"));
				}
			}
			moduleNode = config.getNodes({"modules", "disk"}, targetConfig);
			if(moduleNode.size() == 1 and moduleNode.front() != nullptr){
				bool disk_m = moduleNode.front()->getValue<bool>();
				if(disk_m){
					disk = new Disk(moduleNode.front()->getAttr<int>("fps"));
				}
			}
			moduleNode = config.getNodes({"modules", "time"}, targetConfig);
			if(moduleNode.size() == 1 and moduleNode.front() != nullptr){
				bool time_m = moduleNode.front()->getValue<bool>();
				if(time_m){
					time = new TimeCnt(moduleNode.front()->getAttr<int>("fps"), lang);
				}
			}


			//--------------------------------------------------------------------------------------------------------
			//Load Background data & create background object
			//--------------------------------------------------------------------------------------------------------
			Node* BgNode = config.getNodes({"settings", "background"}, targetConfig).front();
			std::string bgPath = GetWallpaperPath();
			std::string bgMode = "stretch";
			RECT wpRect;
			wpRect.top = wpRect.left = 0;
			wpRect.right = monRect.right-monRect.left;
			wpRect.bottom = monRect.bottom-monRect.top;
			if(BgNode != nullptr){
				//Read config bg path
				std::string configBgPath = BgNode->getValue<std::string>();
				if(configBgPath.size()>0){
					if(!(configBgPath.size()==1 and configBgPath[0] == '0')){ //fix wrong XmlParser Value
						bgPath = configBgPath;
					}
				}
				std::string bgModeConfig = BgNode->getAttr<std::string>("mode");
				if(bgModeConfig == "stretch" or bgModeConfig == "oryginal" or bgModeConfig == "fit-in" or bgModeConfig == "fit-out"){
					bgMode = bgModeConfig;
				}
				//Read display mode: *Stretch to fullscreen, *oryginal image dimmension, *fit to screen
			}
			//std::cout<<bgPath<<std::endl;
			Texture wallpaperTex(bgPath);
			if(bgMode == "oryginal"){
				wpRect.left = (wpRect.right/2)-(wallpaperTex.dimm.width/2);
				wpRect.top = (wpRect.bottom/2)-(wallpaperTex.dimm.height/2);
				wpRect.right = wallpaperTex.dimm.width;
				wpRect.bottom = wallpaperTex.dimm.height;
			} else if(bgMode == "fit-in"){
				int screenWidth = wpRect.right;
				int screenHeight = wpRect.bottom;
				float wprop = (float)screenWidth/wallpaperTex.dimm.width;
				float hprop = (float)screenHeight/wallpaperTex.dimm.height;
				if(wprop <hprop){ //just fix to width
					wpRect.right = wallpaperTex.dimm.width*wprop;
					wpRect.bottom = wallpaperTex.dimm.height*wprop;
				} else {
					wpRect.right = wallpaperTex.dimm.width*hprop;
					wpRect.bottom = wallpaperTex.dimm.height*hprop;
				}
				wpRect.left = (screenWidth/2)-(wpRect.right/2);
				wpRect.top = (screenHeight/2)-(wpRect.bottom/2);
			} else if(bgMode == "fit-out"){
				int screenWidth = wpRect.right;
				int screenHeight = wpRect.bottom;
				float wprop = (float)screenWidth/wallpaperTex.dimm.width;
				float hprop = (float)screenHeight/wallpaperTex.dimm.height;
				if(wprop <hprop){
					wpRect.right = wallpaperTex.dimm.width*hprop;
					wpRect.bottom = wallpaperTex.dimm.height*hprop;
				} else {
					wpRect.right = wallpaperTex.dimm.width*wprop;
					wpRect.bottom = wallpaperTex.dimm.height*wprop;
				}
				wpRect.left = (screenWidth/2)-(wpRect.right/2);
				wpRect.top = (screenHeight/2)-(wpRect.bottom/2);
			}

			Sprite * wallpaper = new Sprite(&wallpaperTex,  &world, PureTranslation(&world, wpRect.left, wpRect.top), 0, wpRect.right, wpRect.bottom);
			//	Primitive(World * world, WorldRect wr, int width, int height, Color color, int flags); //Rectangle
			Primitive * wallpaperOpacity = new Primitive(&world,  WorldRect((int)wpRect.left, (int)wpRect.top,0,0), wpRect.right, wpRect.bottom, Color(0,0,0,255-BgNode->getAttr<int>("opacity")), 0);

			//--------------------------------------------------------------------------------------------------------
			//Create objects that will be displayed
			//--------------------------------------------------------------------------------------------------------

			//Draw container
			std::list<Drawable*> drawable;
			//Update containers
			std::list<ToUpdate*> updatable;

			//Add background to list
			drawable.push_back(wallpaper);
			drawable.push_back(wallpaperOpacity);


			std::list<Node*> objectNode;
			objectNode = config.getNodes({"visualObjects", "object"}, targetConfig);
			if(objectNode.size() > 0 and objectNode.front() != nullptr){
				for(auto & obj : objectNode){

					//Get typename & themeid & value
					std::string typeName =  obj->getAttr<std::string>("type");
					int themeId =  obj->getAttr<int>("useTheme");
					std::string objValue = obj->getValue<std::string>();

					Module * bindedModule = nullptr;
					size_t f;

					if(cpu != nullptr){
						f = objValue.find(cpu->entry);
						if(f != std::string::npos){
							bindedModule = cpu;
						}
					}
					if(ram != nullptr){
						f = objValue.find(ram->entry);
						if(f != std::string::npos){
							bindedModule = ram;
						}
					}
					if(time != nullptr){
						std::regex word_regex("(\\{\\w*\\})");
						auto words_begin = std::sregex_iterator(time->entry.begin(), time->entry.end(), word_regex);
						auto words_end = std::sregex_iterator();
						for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
							std::smatch match = *i;
						    std::string match_str = match.str();
						    f = objValue.find(match_str);
						    if(f != std::string::npos){
								bindedModule = time;
								break;
						    }
						}
					}

					if(typeName == "graph"){
						if(bindedModule != nullptr and bindedModule->moduleType != "time"){
							Graph * tempObj = new Graph(&world, bindedModule, obj->getAttr<int>("x"), obj->getAttr<int>("y"), obj->getAttr<int>("width"), obj->getAttr<int>("height"),themes[themeId]->graphBars, themes[themeId]->graphBase,themes[themeId]->graphBarWidth,themes[themeId]->graphBarSpacing);
							updatable.push_back(tempObj); //For update
							drawable.push_back(tempObj); //For drawing
						}
					} else if(typeName == "text"){
						TextCnt * tempText = new TextCnt(&world, bindedModule, objValue, obj->getAttr<int>("x"), obj->getAttr<int>("y"), obj->getAttr<int>("align"), themes[themeId]->fnt, themes[themeId]->color);
						updatable.push_back(tempText);
						drawable.push_back(tempText);
					} else if(disk != nullptr and typeName == "disk"){
						//disk->Setup(&world, objValue, obj->getAttr<int>("x"), obj->getAttr<int>("y"), obj->getAttr<int>("x_inc"), obj->getAttr<int>("y_inc"), obj->getAttr<int>("width"), themes[themeId]->fnt, themes[themeId]->color);
						//DiskHandler::DiskHandler(World * world, Disk * update, std::string msg, Theme * theme, int x, int y, int x_inc, int y_inc, int width){
						DiskHandler * diskHTemp = new DiskHandler(&world, disk, objValue, themes[themeId], obj->getAttr<int>("x"), obj->getAttr<int>("y"), obj->getAttr<int>("x_inc"), obj->getAttr<int>("y_inc"), obj->getAttr<int>("width"));
						updatable.push_back(diskHTemp);
						drawable.push_back(diskHTemp);
					}


				}
			}

			reload = false;
			//--------------------------------------------------------------------------------------------------------
			//Main main loop
			//--------------------------------------------------------------------------------------------------------
			do {
				glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


				//Handle extensions
				if(manageIcons != nullptr and GetAsyncKeyState(manageIconsKey)){
						manageIcons->Toggle();
						Sleep(100);
				}

				//Update modules data
				if(cpu != nullptr) cpu->Update();
				if(ram != nullptr) ram->Update();
				if(time != nullptr) time->Update();

				//Update drawable
				for(auto & it: updatable){
					it->Update();
				}

				//Draw everything
				for(auto & it: drawable){
					it->Render();
				}

				if(delay > 0)
					Sleep(delay);


				if(reload or exitflag){
					glfwSetWindowShouldClose(ogl.GetHandle(), 1);
					reloaded = true;
				}
				glfwSwapBuffers(ogl.GetHandle());
				glfwPollEvents();
			} while(ogl.IfOpen());


			//--------------------------------------------------------------------------------------------------------
			//Cleanup
			//--------------------------------------------------------------------------------------------------------
			std::cout<<"Cleanup"<<std::endl;
			Shell_NotifyIcon( NIM_DELETE, & nid );

			int c = 0;
			for(auto & it: drawable){
				if(c != 0) delete it;
				c++;
			}
			for(size_t i = 0; i <themes.size(); i++){
				if(themes[i]->fnt != nullptr){
					std::string temp = themes[i]->fnt->path;
					int tsize = themes[i]->fnt->size;
					for(size_t j = i+1; j <themes.size(); j++){
						if(themes[j]->fnt != nullptr and themes[j]->fnt->path == temp and themes[j]->fnt->size == tsize){
							themes[j]->fnt = nullptr;
						}
					}
				}
				if(themes[i] != nullptr) delete themes[i];
			}


			//Extensions
			if(manageIcons != nullptr) delete manageIcons;

			//Modules
			if(cpu != nullptr) delete cpu;
			if(ram != nullptr) delete ram;
			if(time != nullptr) delete time;
			if(disk != nullptr) delete disk;


			//Fix last frame (win10 bug???)
			for(int i = 0; i<10; i++){
				glClear(GL_COLOR_BUFFER_BIT);
				wallpaper->Render();
				glfwSwapBuffers(ogl.GetHandle());
				glfwPollEvents();
				Sleep(33);
			}
			delete wallpaper;

			//Fix AMD
			SetWindowPos((HWND)GetDesktopWindow(), hwnd, 0,0,0,0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER);
			SetForegroundWindow((HWND)GetDesktopWindow());

			//LPWSTR test = L"C:\\Documents and Settings\\Owner\\My Documents\\Wallpapers\\wallpaper.png";
			int result = SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (VOID*)GetWallpaperPath().c_str(), SPIF_UPDATEINIFILE);

			//glfwDestroyWindow(ogl.GetHandle());

		//--------------------------------------------------------------------------------------------------------
		//Just throwing exceptions
		//--------------------------------------------------------------------------------------------------------
		} else {
			std::cout<<"Exce 2"<<std::endl;
		    throw Exception("File", "config.xml", "Cannot find target sub-config in loaded file.");
			return 2;
		}

	} else {
		std::cout<<"Exce 1"<<std::endl;
	    throw Exception("File", "config.xml", "Cannot find any sub-config in loaded file.");
		return 1;
	}

	}



	return 0;
}
