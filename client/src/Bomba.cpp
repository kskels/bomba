
#include "Bomba.h"
#include "Utils.h"

Bomba::Bomba(irr::IrrlichtDevice* device)
    : device(device) {

    sceneManager = device->getSceneManager();

    // Load graphics used...    
    loadResources();

    rootNode = sceneManager->addEmptySceneNode(0,-1);


    irr::scene::ISceneNode* boxNode = sceneManager->addMeshSceneNode(box, rootNode);
    boxNode->setScale(core::vector3df(0.3f,0.3f,0.3f));

    // initial Lights...
    sceneManager->addLightSceneNode(0, core::vector3df(200, 800, 200), 
				    video::SColorf(0.3f, 0.3f, 0.4f, 0.0f), 800.0f);
    sceneManager->addLightSceneNode(0, core::vector3df(-100, 800, -100), 
				    video::SColorf(0.1f, 0.1f, 0.1f, 0.0f), 800.0f);
    sceneManager->addLightSceneNode(0, core::vector3df(-100, 800, -200), 
				    video::SColorf(0.2f, 0.2f, 0.2f, 0.0f), 800.0f);
    
    //setup Camera...
    camera = sceneManager->addCameraSceneNode(0, core::vector3df(0, 1, -0.4), core::vector3df(0,0,0));
    sceneManager->setActiveCamera(camera);

}

Bomba::~Bomba()
{

}

void Bomba::loadResources()
{

    box = sceneManager->getMesh("../res/box1.dae"); 
    
}



void Bomba::update()
{
    // our funky gameloop...
}

void Bomba::render()
{
}
