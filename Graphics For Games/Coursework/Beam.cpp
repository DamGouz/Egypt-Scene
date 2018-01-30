#include "Beam.h"

Mesh * Beam::bm = NULL;


Beam::Beam() {
	// Optional , uncomment if you want a local origin marker !
	//SetMesh(bm);

	
	bm->SetTexture(SOIL_load_OGL_texture("../../Textures/beam.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 0);
	bm->SetTexture(SOIL_load_OGL_texture("../../Textures/beam.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 1);
	
}

// The update function generates particles until the beam's height reaches the pyramid's
void Beam::Update(float msec) {
	for (vector < SceneNode * >::const_iterator i = this->GetChildIteratorStart(); i != this->GetChildIteratorEnd(); ++i) {
		(*i)->SetTransform((*i)->GetTransform()*Matrix4::Translation(Vector3(rand() % 7 - 3, 0, rand() % 7 - 3)));
	}
	if (height > -10000 && this->playAnimation) {
		height -= msec*3/2;
		SceneNode * bd = new SceneNode(bm, Vector4(0, 0, 0, 1));

		bd->SetModelScale(Vector3(4, 4, 4));
		bd->SetTransform(Matrix4::Translation (Vector3(rand()%30 -15, height, rand() % 30 - 15)));
		AddChild(bd);
	}
	else if (height <= -10000 && this->playAnimation) {
		this->setAnim(false);
		this->setfin(true);
	}
	SceneNode::Update(msec);
}
