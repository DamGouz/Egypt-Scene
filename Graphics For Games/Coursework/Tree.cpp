#include "Tree.h"

Mesh * Tree::cylinder = NULL;
Mesh * Tree::leaf = NULL;


// The tree class creates a simple tree with 0 dimensions but with the leaves looking up top.
Tree::Tree(int x, int y) {
	
	// Optional , uncomment if you want a local origin marker !
	// SetMesh ( cube );
	cylinder->SetTexture(SOIL_load_OGL_texture("../../Textures/yellowstone.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 0);
	target_h = y;
	target_r = x;
	growth_c = 0;
	trunk = new SceneNode(cylinder, Vector4(0.5274, 0.3215, 0.1764, 1)); // Brown !
	trunk->SetModelScale(Vector3(0, 0, 0));
	trunk->SetTransform(Matrix4::Translation(Vector3(0, 0, 0)));
	AddChild(trunk);

	Vector4 leaf_color = Vector4(0.2274, 0.6215, 0.2764, 1);

	for (int i = 0; i < 24; i++) {
		leaf_start[i] = rand() % 50;
		leaf_angle[i] = 120.0f + rand() % 30;
		SceneNode * leaves = new SceneNode(leaf, leaf_color);
		leaves->SetModelScale(Vector3(2*(trunk->GetModelScale()).x, (trunk->GetModelScale()).y/3, 2 * (trunk->GetModelScale()).z));
		leaves->SetTransform(Matrix4::Translation(Vector3((trunk->GetModelScale()).x, 2 * (trunk->GetModelScale()).y, (trunk->GetModelScale()).z)));
		leaves->SetTransform(leaves->GetTransform()*Matrix4::Rotation(i*15.0f, Vector3(0, 1, 0))*Matrix4::Rotation(120.0f + rand() % 30, Vector3(1, 0, 0)));
		trunk->AddChild(leaves);
	}	
}


//During the update function, the trunk grows and the leaves rotate around the x and y axis to create a random tree.
void Tree::Update(float msec) {
	if (this->playAnimation) {
		int pos = 0;
		float h = (trunk->GetModelScale()).y;
		float r = (trunk->GetModelScale()).x;
		if (h < target_h)
			h = h + msec / 100;
		if (r < target_r)
			r = r + msec / 1500;
		trunk->SetModelScale(Vector3(r, h, r));
		if (growth_c <= 100) {
			for (vector < SceneNode * >::const_iterator i = trunk->GetChildIteratorStart(); i != trunk->GetChildIteratorEnd(); ++i) {
				float ang;
				if (growth_c < leaf_start[pos])
					ang = 50;
				else
					ang = growth_c - leaf_start[pos] + 50;
				if (ang > leaf_angle[pos])
					ang = leaf_angle[pos];
				(*i)->SetModelScale(Vector3(2 * r, h / 3, 2 * r));
				(*i)->SetTransform(Matrix4::Translation(Vector3(r, 2 * h, r)));
				(*i)->SetTransform((*i)->GetTransform()*Matrix4::Rotation(pos*15.0f, Vector3(0, 1, 0))*Matrix4::Rotation(ang, Vector3(1, 0, 0)));
				pos++;
			}
			growth_c += msec / target_h;
		}
		else {
			this->setfin(true);
			this->setAnim(false);
		}
	}
	SceneNode::Update(msec);
}