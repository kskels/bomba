#ifndef _BOMBA_H_
#define _BOMBA_H_

#include <irrlicht/irrlicht.h>


class Bomba
{

 public:

    Bomba(irr::IrrlichtDevice* device);
    ~Bomba();
    
    void update();
    void render();

 private:

    irr::IrrlichtDevice* device;
    irr::scene::ISceneManager* sceneManager;

    void loadResources();

    irr::scene::IMesh* box;

    irr::scene::ISceneNode* rootNode;
    irr::scene::ICameraSceneNode* camera;
    
    // todo: make lightDeque...
    //irr::scene::ILightNode* light;

};

#endif //_BOMBA_H_
