#include "Room.h"

Mesh * Room::room = NULL;
Mesh * Room::f = NULL;
Mesh * Room::cyl = NULL;

// x,y,z are the dimensions of the room. I do not translate each room on the renderer because I have torches.
Room::Room(float x, float y, float z, int draw_bin) {
	// Optional , uncomment if you want a local origin marker !
	SetMesh(room);
	
	SceneNode * base1 = new SceneNode(cyl, Vector4(0.9, 0.66, 0.62, 1));
	SceneNode * fire1 = new SceneNode(f, Vector4(0.9, 0.66, 0.62, 1));
	
	SceneNode * base2 = new SceneNode(cyl, Vector4(0.9, 0.66, 0.62, 1));
	SceneNode * fire2 = new SceneNode(f, Vector4(0.9, 0.66, 0.62, 1));
	
	SceneNode * base3 = new SceneNode(cyl, Vector4(0.9, 0.66, 0.62, 1));
	SceneNode * fire3 = new SceneNode(f, Vector4(0.9, 0.66, 0.62, 1));

	SceneNode * base4 = new SceneNode(cyl, Vector4(0.9, 0.66, 0.62, 1));
	SceneNode * fire4 = new SceneNode(f, Vector4(0.9, 0.66, 0.62, 1));

	

	base1->SetModelScale(Vector3(10, 50, 10));
	fire1->SetModelScale(Vector3(10, 20, 10));
	base1->SetTransform(Matrix4::Translation(Vector3(-10, y / 10, -10)));
	fire1->SetTransform(Matrix4::Translation(Vector3(0, y / 10 + 110, 0)));
	base2->SetModelScale(Vector3(10, 50, 10));
	fire2->SetModelScale(Vector3(10, 20, 10));
	base2->SetTransform(Matrix4::Translation(Vector3(-5, y / 10, -2 * z-5)));
	fire2->SetTransform(Matrix4::Translation(Vector3(5, y / 10 + 110, -2 * z - 5))*(Matrix4::Rotation(180.0f, Vector3(0, 1, 0))));
	base3->SetModelScale(Vector3(10, 50, 10));
	fire3->SetModelScale(Vector3(10, 20, 10));
	base3->SetTransform(Matrix4::Translation(Vector3(x-10, y / 10, -z-10)));
	fire3->SetTransform(Matrix4::Translation(Vector3(x, y / 10 + 110, -z))*(Matrix4::Rotation(180.0f, Vector3(0, 1, 0))));
	base4->SetModelScale(Vector3(10, 50, 10));
	fire4->SetModelScale(Vector3(10, 20, 10));
	base4->SetTransform(Matrix4::Translation(Vector3(-x-5, y / 10, -z-5)));
	fire4->SetTransform(Matrix4::Translation(Vector3(-x + 5, y / 10 + 110, -z + 5))*(Matrix4::Rotation(180.0f, Vector3(0, 1, 0))));

	cyl ->SetTexture(SOIL_load_OGL_texture("../../Textures/wood.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 0);
	f->SetTexture(SOIL_load_OGL_texture("../../Textures/fire.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 0);
	
	if (draw_bin & 0b000001) {
		AddChild(base1);
		AddChild(fire1);
	}
	if (draw_bin & 0b000010) {
		AddChild(base3);
		AddChild(fire3);
	}
	if (draw_bin & 0b000100) {
		AddChild(base2);
		AddChild(fire2);
	}
	if (draw_bin & 0b001000) {
		AddChild(base4);
		AddChild(fire4);
	}

	SceneNode * wall_e = new SceneNode(room, Vector4(0.9, 0.66, 0.62, 1));
	SceneNode * wall_n = new SceneNode(room, Vector4(0.2, 0.66, 0.62, 1));
	SceneNode * wall_w = new SceneNode(room, Vector4(0.9, 0.66, 0.62, 1));
	SceneNode * wall_s = new SceneNode(room, Vector4(0.9, 0.66, 0.62, 1));
	SceneNode * wall_b = new SceneNode(room, Vector4(0.9, 0.66, 0.62, 1));
	SceneNode * wall_t = new SceneNode(room, Vector4(0.9, 0.66, 0.62, 1));

	wall_e->side = 0;
	wall_n->side = 0;
	wall_w->side = 0;
	wall_s->side = 0;
	wall_b->side = 1;
	wall_t->side = 2;

	wall_e->SetModelScale(Vector3(x, y, 0));
	wall_n->SetModelScale(Vector3(z, y, 0));
	wall_w->SetModelScale(Vector3(x, y, 0));
	wall_s->SetModelScale(Vector3(z, y, 0));
	wall_b->SetModelScale(Vector3(x, z, 0));
	wall_t->SetModelScale(Vector3(x, z, 0));


	Vector3 textureScalar = Vector3(
		1.0f,
		float(y) / float(max(z, x)),
		0.0f
	);

	wall_e->SetTextureMatrix(Matrix4::Scale(textureScalar));
	wall_n->SetTextureMatrix(Matrix4::Scale(textureScalar));
	wall_w->SetTextureMatrix(Matrix4::Scale(textureScalar));
	wall_s->SetTextureMatrix(Matrix4::Scale(textureScalar));
	wall_b->SetTextureMatrix(Matrix4::Scale(textureScalar));
	wall_t->SetTextureMatrix(Matrix4::Scale(textureScalar));

	wall_e->SetTransform(Matrix4::Translation(Vector3(0, 0, 0)));
	wall_n->SetTransform(Matrix4::Translation(Vector3(x, 0, -z))*(Matrix4::Rotation(90.0f, Vector3(0, 1, 0))));
	wall_w->SetTransform(Matrix4::Translation(Vector3(0, 0, -2*z))*(Matrix4::Rotation(180.0f, Vector3(0, 1, 0))));
	wall_s->SetTransform(Matrix4::Translation(Vector3(-x, 0, -z))*(Matrix4::Rotation(270.0f, Vector3(0, 1, 0))));
	wall_b->SetTransform(Matrix4::Translation(Vector3(0, -y, -z))*(Matrix4::Rotation(180.0f, Vector3(0, -1, 1))));
	wall_t->SetTransform(Matrix4::Translation(Vector3(0, y, -z))*(Matrix4::Rotation(180.0f, Vector3(0, 1, 1))));

	room->SetTexture(SOIL_load_OGL_texture("../../Textures/hiero.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 0);
	room->SetTexture(SOIL_load_OGL_texture("../../Textures/wooden_floor.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 1);
	room->SetTexture(SOIL_load_OGL_texture("../../Textures/wooden_floor.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 2);
	if (draw_bin & 0b000001)
		AddChild(wall_e);
	if (draw_bin & 0b000010)
		AddChild(wall_n);
	if (draw_bin & 0b000100)
		AddChild(wall_w);
	if (draw_bin & 0b001000)
		AddChild(wall_s);
	if (draw_bin & 0b010000)
		AddChild(wall_b);
	if (draw_bin & 0b100000)
		AddChild(wall_t);
}

void Room::Update(float msec) {
	SceneNode::Update(msec);
}