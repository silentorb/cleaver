#include "Cleaver.h"
#include <memory>
#include <SDL2/SDL_syswm.h>

namespace cleaver {

  void Cleaver::initialize() {
    bool result = false;
    try
    {
      Ogre::String config_file = "";
      Ogre::String plugins_file = "";
      Ogre::String log_file = "";

      root = std::unique_ptr<Ogre::Root>(
        new Ogre::Root(config_file, plugins_file, log_file));
      {
        typedef std::vector<Ogre::String> Strings;
        Strings plugin_names;
        plugin_names.push_back("RenderSystem_GL");
//        plugin_names.push_back("RenderSystem_Direct3D9");
//        plugin_names.push_back("Plugin_ParticleFX");
        plugin_names.push_back("Plugin_CgProgramManager");
        //plugin_names.push_back("Plugin_PCZSceneManager");
        //plugin_names.push_back("Plugin_OctreeZone");
        plugin_names.push_back("Plugin_OctreeSceneManager");
        //plugin_names.push_back("Plugin_BSPSceneManager");

        {
          Strings::iterator lIter = plugin_names.begin();
          Strings::iterator lIterEnd = plugin_names.end();
          for(;lIter != lIterEnd; lIter++)
          {
            Ogre::String& lPluginName = (*lIter);
            bool lIsInDebugMode = OGRE_DEBUG_MODE;
            if(lIsInDebugMode)
            {
              lPluginName.append("_d");
            }
            root->loadPlugin(lPluginName);
          }
        }
      }

      int width = 800, height = 600;

      SDL_Init(SDL_INIT_VIDEO);   // Initialize SDL2

      // Create an application window with the following settings:
      sdl_window = SDL_CreateWindow(
        "An SDL2 window",                  //    window title
        SDL_WINDOWPOS_UNDEFINED,           //    initial x position
        SDL_WINDOWPOS_UNDEFINED,           //    initial y position
        width,                               //    width, in pixels
        height,                               //    height, in pixels
        SDL_WINDOW_SHOWN //    flags - see below
      );

      // Check that the window was successfully made
      if(sdl_window==NULL){
        // In the event that the window could not be made...
        std::cout << "Could not create window: " << SDL_GetError() << '\n';
        throw new std::runtime_error("Could not create window.");
      }

      struct SDL_SysWMinfo wmInfo;
      SDL_VERSION(&wmInfo.version);

      if (SDL_GetWindowWMInfo(sdl_window, &wmInfo) == SDL_FALSE)
        throw std::runtime_error("Couldn't get WM Info.");

      Ogre::String winHandle;

      switch (wmInfo.subsystem)
      {
        case SDL_SYSWM_WINDOWS:
          winHandle = Ogre::StringConverter::toString((unsigned long)wmInfo.info.win.window);
          break;
        default:
          throw std::runtime_error("Unexpected WM!");
          break;
      }

      {
        // the root provide a method if you want to select
        // the rendersystem and its options visually (lRoot->showConfigDialog()).
        // in that case, you don't need to set the render system manually

        const Ogre::RenderSystemList& lRenderSystemList = root->getAvailableRenderers();
        if( lRenderSystemList.size() == 0 )
        {
//          MESSAGE_ERROR("Sorry, no rendersystem was found.");
          throw std::runtime_error("Unable to initialize render system.");
        }

        Ogre::RenderSystem *lRenderSystem = lRenderSystemList[0];
        root->setRenderSystem(lRenderSystem);
      }

      {
        // name of the automatically generated window. empty for me.
        Ogre::String lWindowTitle = "";
        // custom capabilities of the rendersystem. It's a feature for advanced use.
        Ogre::String lCustomCapacities = "";
        root->initialise(false);
      }

      {
        Ogre::String lWindowTitle = "Marloth Rogue";
        bool fullscreen = false;
        Ogre::NameValuePairList arguments;
        arguments["FSAA"] = "0";
        // vertical synchronisation will prevent some image-tearing, but also
        // will provide smooth framerate in windowed mode.(check wikipedia if needed).
        arguments["vsync"] = "true";
        arguments["parentWindowHandle"] = winHandle;
        arguments["left"] = "0";
        arguments["top"] = "0";
        ogre_window = root->createRenderWindow(lWindowTitle, width, height, fullscreen, &arguments);
        ogre_window->setPriority(1);
      }
      result = true;

      root->clearEventTimes();

    } catch(Ogre::Exception &e)
    {
//      MESSAGE_ERROR(e.what());
      throw new std::runtime_error(e.what());
      result = false;
    } catch(std::exception &e)
    {
//      MESSAGE_ERROR(e.what());
      throw new std::runtime_error(e.what());
      result = false;
    }
  }
}