#include <cleaver/Cleaver.h>
#include "cleaver/Barracks.h"
#include "lookinglass/Element.h"

using namespace lookinglass;

namespace cleaver {
  namespace Barracks {

    void create_camera(Cleaver *cleaver, const Element &element) {
      auto name = "Camera" + std::to_string(++cleaver->camera_index);
      auto camera = cleaver->scene_manager->createCamera(name);
      camera->setPosition(Ogre::Vector3(0, 300, 500));
      camera->lookAt(Ogre::Vector3(0, 0, 0));
      camera->setNearClipDistance(5);
      cleaver->viewport->setCamera(camera);
      cleaver->element_map[&element] = name;
    }

    void create_model(Cleaver *cleaver, const Element &element) {
      auto mesh = cleaver->createColourCube();

      Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create(
        "Test/ColourTest", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
      material->getTechnique(0)->getPass(0)->setVertexColourTracking(Ogre::TVC_AMBIENT);

      Ogre::Entity *thisEntity = cleaver->scene_manager->createEntity("cc", mesh->getName());
      thisEntity->setMaterialName("Test/ColourTest");
      Ogre::SceneNode *thisSceneNode = cleaver->scene_manager->getRootSceneNode()->createChildSceneNode();
      thisSceneNode->setPosition(-35, 0, 0);
      thisSceneNode->attachObject(thisEntity);
      cleaver->element_map[&element] = mesh->getName();
    }
  }

}