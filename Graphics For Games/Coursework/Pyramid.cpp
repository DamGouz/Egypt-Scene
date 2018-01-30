#include "Pyramid.h"

Mesh * Pyramid::pyr = NULL;


// Simple pyramid. 2 different textures for the sand and the brick effect.
Pyramid::Pyramid() {
	// Optional , uncomment if you want a local origin marker !
	SetMesh ( pyr );

	SceneNode * body = new SceneNode(pyr, Vector4(0, 0, 0, 1));
	
	body -> SetModelScale(Vector3(1000, 1000, 750));

	body -> SetTransform(Matrix4::Rotation(-90.0f,Vector3(1, 0, 0)));
	
	pyr -> SetTexture(SOIL_load_OGL_texture("../../Textures/yellowstone.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 0);
	pyr -> SetTexture(SOIL_load_OGL_texture("../../Textures/brick_joint.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 1);
	AddChild(body);
}

void Pyramid::Update(float msec) {

	SceneNode::Update(msec);
}
