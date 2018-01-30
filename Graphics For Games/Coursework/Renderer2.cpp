#include "Renderer2.h"

Renderer2::Renderer2(Window & parent) : OGLRenderer(parent) {
	Room::CreateRoom(); // Important !
	camera = new Camera();
	wallShader = new Shader("../../Shaders/InsideVertex.glsl", "../../Shaders/InsideFragment.glsl");
	if (!wallShader->LinkProgram()) {
		return;
	}
	projMatrix = Matrix4::Perspective(1.0f, 100000.0f, (float)width / (float)height, 45.0f);

	camera->SetPosition(Vector3(-5000, 0, -1000));
	camera->SetPitch(0);
	camera->SetYaw(-90);

	root = new SceneNode();
	Room * room1 = new Room(5000, 500, 1000, 0b111101);
	room1->SetTransform(Matrix4::Translation(Vector3(0, 0, 0)));
	Room * room2 = new Room(1000, 500, 1000, 0b110011);
	room2->SetTransform(Matrix4::Translation(Vector3(6000, 0, 0)));
	Room * room3 = new Room(1000, 500, 5000, 0b111010);
	room3->SetTransform(Matrix4::Translation(Vector3(6000, 0, -2000)));
	Room * room4 = new Room(1000, 500, 1000, 0b111100);
	room4->SetTransform(Matrix4::Translation(Vector3(6000, 0, -12000)));
	Room * room5 = new Room(8000, 500, 1000, 0b110101);
	room5->SetTransform(Matrix4::Translation(Vector3(15000, 0, -12000)));
	Room * main_room = new Room(8000, 2000, 8000, 0b111111);
	main_room->SetTransform(Matrix4::Translation(Vector3(31000, 1500, -6000)));
	for (int i = 0; i < 3; i++) {
		SetTextureRepeating(room1->GetMesh()->GetTexture(i), true);
		SetTextureRepeating(room2->GetMesh()->GetTexture(i), true);
		SetTextureRepeating(room3->GetMesh()->GetTexture(i), true);
		SetTextureRepeating(room4->GetMesh()->GetTexture(i), true);
		SetTextureRepeating(room5->GetMesh()->GetTexture(i), true);
		SetTextureRepeating(main_room->GetMesh()->GetTexture(i), true);
	}
	root->AddChild(room1);
	root->AddChild(room2);
	root->AddChild(room3);
	root->AddChild(room4);
	root->AddChild(room5);
	root->AddChild(main_room);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	init = true;
}

Renderer2 ::~Renderer2(void) {
	delete root;
	delete camera;
	Pyramid::DeletePyramid(); // Also important !
}

void Renderer2::UpdateScene(float msec) {
	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
	root->Update(msec);
}

void Renderer2::RenderScene() {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	SetCurrentShader(wallShader);
	glUseProgram(wallShader->GetProgram());
	UpdateShaderMatrices();
	glUniform1i(glGetUniformLocation(wallShader->GetProgram(), "roomTex1"), 0);
	glUniform1i(glGetUniformLocation(wallShader->GetProgram(), "roomTex2"), 1);
	glUniform1i(glGetUniformLocation(wallShader->GetProgram(), "roomTex3"), 2);
	DrawNode(root);

	glUseProgram(0);
	SwapBuffers();
}

void Renderer2::DrawNode(SceneNode * n) {
	if (n->GetMesh()) {
		Matrix4 transform = n->GetWorldTransform()*Matrix4::Scale(n->GetModelScale());
		glUniformMatrix4fv(glGetUniformLocation(wallShader->GetProgram(), "modelMatrix"), 1, false, (float *)& transform);
		glUniform1i(glGetUniformLocation(wallShader->GetProgram(), "useTexture"), (int)n->GetMesh()->GetTexture(0));
		glUniform1i(glGetUniformLocation(wallShader->GetProgram(), "side"), n->side);
		n->Draw(*this);
	}

	for (vector < SceneNode * >::const_iterator i = n->GetChildIteratorStart(); i != n->GetChildIteratorEnd(); ++i) {
		DrawNode(*i);
	}
}
