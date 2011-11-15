#ifndef _UTILS_H_
#define _UTILS_H_

using namespace irr;

static scene::IMeshSceneNode* fileAddNode(scene::ISceneManager* manager, const char* filename, scene::ISceneNode* parent = 0, int id= -1) {
    scene::IMesh* mesh = manager->getMesh(filename);
    if(!mesh) {
	//LOGV("Failed to load: %s", pFilename);
	return 0;
    }
    // temp: setup some default materials...
    scene::IMeshSceneNode* node = manager->addMeshSceneNode( mesh, parent, id);
    node->setMaterialFlag(video::EMF_LIGHTING, true);
    node->setMaterialFlag(video::EMF_GOURAUD_SHADING, true);
    node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, true);
    node->setMaterialFlag(video::EMF_ANTI_ALIASING, true);
    node->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true);
    return node;
}

#endif //_UTILS_H_
