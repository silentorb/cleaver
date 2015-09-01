#include <memory>
#include <SDL2/SDL_syswm.h>
#include "cleaver/Cleaver.h"

namespace cleaver {

  Cleaver::Cleaver() {
    root = new Ogre::Root("", "", "");
  }

  Cleaver::~Cleaver() {
    delete root;
    root = NULL;
  }


  void Cleaver::start() {
    bool result = false;
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
        for (; lIter != lIterEnd; lIter++) {
          Ogre::String &lPluginName = (*lIter);
          bool lIsInDebugMode = OGRE_DEBUG_MODE;
          lIsInDebugMode = true;
          if (lIsInDebugMode) {
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
    if (sdl_window == NULL) {
      // In the event that the window could not be made...
      std::cout << "Could not create window: " << SDL_GetError() << '\n';
      throw new std::runtime_error("Could not create window.");
    }

    struct SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);

    if (SDL_GetWindowWMInfo(sdl_window, &wmInfo) == SDL_FALSE)
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

      const Ogre::RenderSystemList &lRenderSystemList = root->getAvailableRenderers();
      if (lRenderSystemList.size() == 0) {
//          MESSAGE_ERROR("Sorry, no rendersystem was found.");
        throw std::runtime_error("Unable to start render system.");
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

    viewport = ogre_window->addViewport(NULL);
    //viewport = window->addViewport(NULL);
    viewport->setOverlaysEnabled(true);
    viewport->setClearEveryFrame(true);
    viewport->setBackgroundColour(Ogre::ColourValue(0, 1, 1));
    root->clearEventTimes();

    auto scene_manager = root->createSceneManager(Ogre::ST_GENERIC, "Main_Scene");
    auto camera = scene_manager->createCamera("Cam");
    camera->setPosition(Ogre::Vector3(0, 300, 500));
    camera->lookAt(Ogre::Vector3(0, 0, 0));
    camera->setNearClipDistance(5);
    viewport->setCamera(camera);

    scene_manager->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
    auto mesh = createColourCube();

    Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create(
      "Test/ColourTest", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    material->getTechnique(0)->getPass(0)->setVertexColourTracking(Ogre::TVC_AMBIENT);

    Ogre::Entity *thisEntity = scene_manager->createEntity("cc", mesh->getName());
    thisEntity->setMaterialName("Test/ColourTest");
    Ogre::SceneNode *thisSceneNode = scene_manager->getRootSceneNode()->createChildSceneNode();
    thisSceneNode->setPosition(-35, 0, 0);
    thisSceneNode->attachObject(thisEntity);

    ogre_window->setActive(true);
    ogre_window->setAutoUpdated(false);

  }

  void Cleaver::stop() {

  }

  int Cleaver::update() {
    if (ogre_window->isClosed())
      return 1;

    SDL_Event event;
    if (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT)
        return 1;
    }

    ogre_window->update(false);
    root->renderOneFrame();
    ogre_window->swapBuffers();
    //Ogre::WindowEventUtilities::messagePump();

//    if (should_close)
//      return 1;

    return 0;
  }

  static unsigned long mesh_index = 0;

  using namespace Ogre;

  Ogre::MeshPtr Cleaver::createColourCube() {
    unsigned long index = ++mesh_index; // Store for thread safety

    /// Create the mesh via the MeshManager
    Ogre::MeshPtr mesh = MeshManager::getSingleton().createManual("mesh_" + std::to_string(index), "General");

    /// Create one submesh
    SubMesh *sub = mesh->createSubMesh();

    const float sqrt13 = 0.577350269f; /* sqrt(1/3) */

    /// Define the vertices (8 vertices, each have 3 floats for position and 3 for normal)
    const size_t nVertices = 8;
    const size_t vbufCount = 3 * 2 * nVertices;
    float vertices[vbufCount] = {
      -100.0, 100.0, -100.0,        //0 position
      -sqrt13, sqrt13, -sqrt13,     //0 normal
      100.0, 100.0, -100.0,         //1 position
      sqrt13, sqrt13, -sqrt13,      //1 normal
      100.0, -100.0, -100.0,        //2 position
      sqrt13, -sqrt13, -sqrt13,     //2 normal
      -100.0, -100.0, -100.0,       //3 position
      -sqrt13, -sqrt13, -sqrt13,    //3 normal
      -100.0, 100.0, 100.0,         //4 position
      -sqrt13, sqrt13, sqrt13,      //4 normal
      100.0, 100.0, 100.0,          //5 position
      sqrt13, sqrt13, sqrt13,       //5 normal
      100.0, -100.0, 100.0,         //6 position
      sqrt13, -sqrt13, sqrt13,      //6 normal
      -100.0, -100.0, 100.0,        //7 position
      -sqrt13, -sqrt13, sqrt13,     //7 normal
    };

    RenderSystem *rs = Root::getSingleton().getRenderSystem();
    RGBA colours[nVertices];
    RGBA *pColour = colours;
    // Use render system to convert colour value since colour packing varies
    rs->convertColourValue(ColourValue(1.0, 0.0, 0.0), pColour++); //0 colour
    rs->convertColourValue(ColourValue(1.0, 1.0, 0.0), pColour++); //1 colour
    rs->convertColourValue(ColourValue(0.0, 1.0, 0.0), pColour++); //2 colour
    rs->convertColourValue(ColourValue(0.0, 0.0, 0.0), pColour++); //3 colour
    rs->convertColourValue(ColourValue(1.0, 0.0, 1.0), pColour++); //4 colour
    rs->convertColourValue(ColourValue(1.0, 1.0, 1.0), pColour++); //5 colour
    rs->convertColourValue(ColourValue(0.0, 1.0, 1.0), pColour++); //6 colour
    rs->convertColourValue(ColourValue(0.0, 0.0, 1.0), pColour++); //7 colour

    /// Define 12 triangles (two triangles per cube face)
    /// The values in this table refer to vertices in the above table
    const size_t ibufCount = 36;
    unsigned short faces[ibufCount] = {
      0, 2, 3,
      0, 1, 2,
      1, 6, 2,
      1, 5, 6,
      4, 6, 5,
      4, 7, 6,
      0, 7, 4,
      0, 3, 7,
      0, 5, 1,
      0, 4, 5,
      2, 7, 3,
      2, 6, 7
    };

    /// Create vertex data structure for 8 vertices shared between submeshes
    mesh->sharedVertexData = new VertexData();
    mesh->sharedVertexData->vertexCount = nVertices;

    /// Create declaration (memory format) of vertex data
    VertexDeclaration *decl = mesh->sharedVertexData->vertexDeclaration;
    size_t offset = 0;
    // 1st buffer
    decl->addElement(0, offset, VET_FLOAT3, VES_POSITION);
    offset += VertexElement::getTypeSize(VET_FLOAT3);
    decl->addElement(0, offset, VET_FLOAT3, VES_NORMAL);
    offset += VertexElement::getTypeSize(VET_FLOAT3);
    /// Allocate vertex buffer of the requested number of vertices (vertexCount)
    /// and bytes per vertex (offset)
    HardwareVertexBufferSharedPtr vbuf =
      HardwareBufferManager::getSingleton().createVertexBuffer(
        offset, mesh->sharedVertexData->vertexCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY);
    /// Upload the vertex data to the card
    vbuf->writeData(0, vbuf->getSizeInBytes(), vertices, true);

    /// Set vertex buffer binding so buffer 0 is bound to our vertex buffer
    VertexBufferBinding *bind = mesh->sharedVertexData->vertexBufferBinding;
    bind->setBinding(0, vbuf);

    // 2nd buffer
    offset = 0;
    decl->addElement(1, offset, VET_COLOUR, VES_DIFFUSE);
    offset += VertexElement::getTypeSize(VET_COLOUR);
    /// Allocate vertex buffer of the requested number of vertices (vertexCount)
    /// and bytes per vertex (offset)
    vbuf = HardwareBufferManager::getSingleton().createVertexBuffer(
      offset, mesh->sharedVertexData->vertexCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY);
    /// Upload the vertex data to the card
    vbuf->writeData(0, vbuf->getSizeInBytes(), colours, true);

    /// Set vertex buffer binding so buffer 1 is bound to our colour buffer
    bind->setBinding(1, vbuf);

    /// Allocate index buffer of the requested number of vertices (ibufCount)
    HardwareIndexBufferSharedPtr ibuf = HardwareBufferManager::getSingleton().
      createIndexBuffer(
      HardwareIndexBuffer::IT_16BIT,
      ibufCount,
      HardwareBuffer::HBU_STATIC_WRITE_ONLY);

    /// Upload the index data to the card
    ibuf->writeData(0, ibuf->getSizeInBytes(), faces, true);

    /// Set parameters of the submesh
    sub->useSharedVertices = true;
    sub->indexData->indexBuffer = ibuf;
    sub->indexData->indexCount = ibufCount;
    sub->indexData->indexStart = 0;

    /// Set bounding information (for culling)
    mesh->_setBounds(AxisAlignedBox(-100, -100, -100, 100, 100, 100));
    mesh->_setBoundingSphereRadius(Math::Sqrt(3 * 100 * 100));

    /// Notify -Mesh object that it has been loaded
    mesh->load();

    return mesh;
  }

  void Cleaver::add_element(lookinglass::Element element) {

  }

  void Cleaver::delete_element(lookinglass::Element element) {

  }
}