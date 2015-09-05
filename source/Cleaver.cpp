
#include <memory>
#include <SDL2/SDL_syswm.h>
#include "cleaver/Cleaver.h"
#include "cleaver/Barracks.h"
#include "cleaver/Hall.h"

namespace cleaver {

  Cleaver::Cleaver() {
    camera_index = 0;
    hall = new Hall();
    hall->root = new Ogre::Root("", "", "");
    barracks_functions[Element_Type::model] = Barracks::create_model;
    barracks_functions[Element_Type::camera] = Barracks::create_camera;
  }

  Cleaver::~Cleaver() {
    delete hall->root;
    hall->root = NULL;
    delete hall;
    hall = NULL;
  }


  void Cleaver::start() {

    bool result = false;
    {
      Ogre::LogManager::getSingleton().setLogDetail(Ogre::LL_LOW);
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
        for (; lIter != lIterEnd; lIter++) {
          Ogre::String &lPluginName = (*lIter);
          bool lIsInDebugMode = OGRE_DEBUG_MODE;
          lIsInDebugMode = true;
          if (lIsInDebugMode) {
            lPluginName.append("_d");
          }
          hall->root->loadPlugin(lPluginName);
        }
      }
    }

    int width = 800, height = 600;

    SDL_Init(SDL_INIT_VIDEO);   // Initialize SDL2

    // Create an application window with the following settings:
    hall->sdl_window = SDL_CreateWindow(
      "An SDL2 window",                  //    window title
      SDL_WINDOWPOS_UNDEFINED,           //    initial x position
      SDL_WINDOWPOS_UNDEFINED,           //    initial y position
      width,                               //    width, in pixels
      height,                               //    height, in pixels
      SDL_WINDOW_SHOWN //    flags - see below
    );

    // Check that the window was successfully made
    if (hall->sdl_window == NULL) {
      // In the event that the window could not be made...
      std::cout << "Could not create window: " << SDL_GetError() << '\n';
      throw new std::runtime_error("Could not create window.");
    }

    struct SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);

    if (SDL_GetWindowWMInfo(hall->sdl_window, &wmInfo) == SDL_FALSE)
      throw std::runtime_error("Couldn't get WM Info.");

    Ogre::String winHandle;

    switch (wmInfo.subsystem) {
      case SDL_SYSWM_WINDOWS:
        winHandle = Ogre::StringConverter::toString((unsigned long) wmInfo.info.win.window);
        break;
      default:
        throw std::runtime_error("Unexpected WM!");
        break;
    }

    {
      // the root provide a method if you want to select
      // the rendersystem and its options visually (lRoot->showConfigDialog()).
      // in that case, you don't need to set the render system manually

      const Ogre::RenderSystemList &lRenderSystemList = hall->root->getAvailableRenderers();
      if (lRenderSystemList.size() == 0) {
//          MESSAGE_ERROR("Sorry, no rendersystem was found.");
        throw std::runtime_error("Unable to start render system.");
      }

      Ogre::RenderSystem *lRenderSystem = lRenderSystemList[0];
      hall->root->setRenderSystem(lRenderSystem);
    }

    {
      // name of the automatically generated window. empty for me.
      Ogre::String lWindowTitle = "";
      // custom capabilities of the rendersystem. It's a feature for advanced use.
      Ogre::String lCustomCapacities = "";
      hall->root->initialise(false);
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
      hall->ogre_window = hall->root->createRenderWindow(lWindowTitle, width, height, fullscreen, &arguments);
      hall->ogre_window->setPriority(1);
    }
    result = true;

    hall->viewport = hall->ogre_window->addViewport(NULL);
    hall->viewport->setOverlaysEnabled(true);
    hall->viewport->setClearEveryFrame(true);
    hall->viewport->setBackgroundColour(Ogre::ColourValue(0, 1, 1));
    hall->root->clearEventTimes();

    hall->scene_manager = hall->root->createSceneManager(Ogre::ST_GENERIC, "Main_Scene");
    hall->scene_manager->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

//    auto camera = scene_manager->createCamera("Cam");
//    camera->setPosition(Ogre::Vector3(0, 300, 500));
//    camera->lookAt(Ogre::Vector3(0, 0, 0));
//    camera->setNearClipDistance(5);
//    viewport->setCamera(camera);

    hall->ogre_window->setActive(true);
    hall->ogre_window->setAutoUpdated(false);

  }

  void Cleaver::stop() {
    hall->ogre_window->destroy();
  }

  int Cleaver::update() {
    if (hall->ogre_window->isClosed())
      return 1;

    SDL_Event event;
    if (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT)
        return 1;
    }

    hall->ogre_window->update(false);
    hall->root->renderOneFrame();
    hall->ogre_window->swapBuffers();
    //Ogre::WindowEventUtilities::messagePump();

//    if (should_close)
//      return 1;

    return 0;
  }


  using namespace Ogre;


}