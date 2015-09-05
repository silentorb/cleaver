#include "cleaver/Hall.h"
#include <cleaver/Cleaver.h>
#include "cleaver/Barracks.h"
#include "OgreMeshManager.h"
#include "OgreEntity.h"
#include "OgreSubMesh.h"

using namespace lookinglass;

namespace cleaver {

  static unsigned long mesh_index = 0;

  namespace Barracks {

    void create_camera(Cleaver *cleaver, const Element &element) {
      auto name = "Camera" + std::to_string(++cleaver->camera_index);
      auto hall = cleaver->hall;
      auto camera = hall->scene_manager->createCamera(name);
      camera->setPosition(Ogre::Vector3(0, 300, 500));
      camera->lookAt(Ogre::Vector3(0, 0, 0));
      camera->setNearClipDistance(5);
      hall->viewport->setCamera(camera);
      cleaver->element_map[&element] = name;
    }

    void create_model(Cleaver *cleaver, const Element &element) {
      auto mesh = createColourCube();
      auto hall = cleaver->hall;

      Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create(
        "Test/ColourTest", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
      material->getTechnique(0)->getPass(0)->setVertexColourTracking(Ogre::TVC_AMBIENT);

      Ogre::Entity *thisEntity = hall->scene_manager->createEntity("cc", mesh->getName());
      thisEntity->setMaterialName("Test/ColourTest");
      Ogre::SceneNode *thisSceneNode = hall->scene_manager->getRootSceneNode()->createChildSceneNode();
      thisSceneNode->setPosition(-35, 0, 0);
      thisSceneNode->attachObject(thisEntity);
      cleaver->element_map[&element] = mesh->getName();
    }
  }

  using namespace Ogre;

  Ogre::MeshPtr createColourCube() {
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

  void Cleaver::add_element(const Element_Pointer &element) {
    elements.push_back(element);
    barracks_functions[element->type](this, *element);
  }

  void Cleaver::delete_element(const Element_Pointer &element) {
    elements.erase(std::remove(elements.begin(), elements.end(), element), elements.end());
  }
}