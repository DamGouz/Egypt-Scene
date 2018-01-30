#include "Plant.h"


Mesh * Plant::leaf = NULL;


// Simple plant. 3 Identical leaves rotated around, to make it seem like a plant.
Plant::Plant(int x) {

	// Optional , uncomment if you want a local origin marker !
	// SetMesh ( cube );
	leaf->SetTexture(SOIL_load_OGL_texture("../../Textures/yellowstone.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 0);
	
	SceneNode * leaves1 = new SceneNode(leaf, Vector4(0.2274, 0.9215, 0.1764, 1)); 
	leaves1->SetModelScale(Vector3(2 * x, 8 * x, 2 * x));
	leaves1->SetTransform(Matrix4::Translation(Vector3(0, 0, 0))*Matrix4::Rotation(0, Vector3(0, 1, 0))*Matrix4::Rotation(90, Vector3(1, 0, 0)));
	SceneNode * leaves2 = new SceneNode(leaf, Vector4(0.2274, 0.9215, 0.1764, 1));
	leaves2->SetModelScale(Vector3(2 * x, 8 * x, 2 * x));
	leaves2->SetTransform(Matrix4::Translation(Vector3(0, 0, 0))*Matrix4::Rotation(120,Vector3(0, 1, 0))*Matrix4::Rotation(90, Vector3(1, 0, 0)));
	SceneNode * leaves3 = new SceneNode(leaf, Vector4(0.2274, 0.9215, 0.1764, 1));
	leaves3->SetModelScale(Vector3(2 * x, 8 * x, 2 * x));
	leaves3->SetTransform(Matrix4::Translation(Vector3(0, 0, 0))*Matrix4::Rotation(-120, Vector3(0, 1, 0))*Matrix4::Rotation(90, Vector3(1, 0, 0)));
	
	
	AddChild(leaves1);
	AddChild(leaves2);
	AddChild(leaves3);
	
}

void Plant::Update(float msec) {
	if (!this->fin())
		this->setfin(true);
	SceneNode::Update(msec);
}