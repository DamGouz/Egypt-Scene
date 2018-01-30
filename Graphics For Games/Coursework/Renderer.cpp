#include "Renderer.h"


Renderer::Renderer(Window & parent) : OGLRenderer(parent) {
	shadowShader = new Shader(SHADERDIR"shadowVert.glsl", SHADERDIR"shadowFrag.glsl");

	if (!shadowShader->LinkProgram()) {
		return;
	}

	//Shadow Bindings
	glGenTextures(1, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE,
		GL_COMPARE_R_TO_TEXTURE);

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &shadowFBO);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTex, 0);
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	
	srand((unsigned)time(NULL));
	camera = new Camera();
	projMatrix = Matrix4::Perspective(1.0f, 100000.0f, (float)width / (float)height, 45.0f);
	quad = Mesh::GenerateQuad();
	light = new Light(Vector3(10000, 4000.0f, 10000), Vector4(0.9f, 0.9f, 1.0f, 1), 500000);
	setTorchlight();
	reflectShader = new Shader(SHADERDIR"PerPixelVertex.glsl", SHADERDIR"PerPixelFragment.glsl");
	skyboxShader = new Shader(SHADERDIR"skyboxVertex.glsl", SHADERDIR"skyboxFragment.glsl");
	lightShader = new Shader(SHADERDIR"shadowscenevert.glsl", SHADERDIR"shadowscenefrag.glsl");
	quad->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"water.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 0);
	cubeMap = SOIL_load_OGL_cubemap(
		TEXTUREDIR"sahara_ft.tga", TEXTUREDIR"sahara_bk.tga",
		TEXTUREDIR"sahara_up.tga", TEXTUREDIR"sahara_dn.tga",
		TEXTUREDIR"sahara_rt.tga", TEXTUREDIR"sahara_lf.tga",
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID, 0);
	if (!reflectShader->LinkProgram() || !lightShader->LinkProgram() || !skyboxShader->LinkProgram()) {
		return;
	}

	// The variable debug loads only certain scenes. So, by having debug set to 0b111 loads all 3 of them in the renderer.
	debug = 0b111;

	if (debug & 0b001) {
		heightMap = new HeightMap("../../Textures/terrain.raw");
		pyramidShader = new Shader("../../Shaders/PyramidVertex.glsl", "../../Shaders/PyramidFragment.glsl");
		
		if (!pyramidShader->LinkProgram()) {
			return;
		}
		root = new SceneNode();
		heightMap->SetTexture(SOIL_load_OGL_texture("../../Textures/sand.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 0);
		heightMap->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"desertbump.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
		Pyramid::CreatePyramid();
		Beam::CreateBeam();

		beam1 = new Beam();
		beam1->SetTransform(Matrix4::Translation(Vector3(13990, 10700, 14010)));
		root->AddChild(beam1);
		beam2 = new Beam();
		beam2->SetTransform(Matrix4::Translation(Vector3(14990, 10700, 15010)));
		root->AddChild(beam2);
		
		Pyramid * pyramid = new Pyramid();
		Pyramid * pyramid2 = new Pyramid();
		pyramid2->SetTransform(Matrix4::Translation(Vector3(14000, 780, 14000)));
		pyramid->SetTransform(Matrix4::Translation(Vector3(15000, 780, 15000)));
		SetTextureRepeating(heightMap->GetTexture(0), true);
		SetTextureRepeating(heightMap->GetBumpMap(), true);
		SetTextureRepeating(pyramid->GetMesh()->GetTexture(0), true);
		SetTextureRepeating(pyramid->GetMesh()->GetTexture(1), true);
		SetTextureRepeating(pyramid2->GetMesh()->GetTexture(0), true);
		SetTextureRepeating(pyramid2->GetMesh()->GetTexture(1), true);
		root->AddChild(pyramid);
		root->AddChild(pyramid2);
	}

	if (debug & 0b010) {
		root2 = new SceneNode();
		wallShader = new Shader("../../Shaders/InsideVertex.glsl", "../../Shaders/InsideFragment.glsl");
		if (!wallShader->LinkProgram()) {
			return;
		}
		Room::CreateRoom();
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
		root2->AddChild(room1);
		root2->AddChild(room2);
		root2->AddChild(room3);
		root2->AddChild(room4);
		root2->AddChild(room5);
		root2->AddChild(main_room);

	}
	if (debug & 0b100) {
		
		
		Plant::CreatePlant();
		Tree::CreateTree();

		treeShader = new Shader("../../Shaders/TreeVertex.glsl", "../../Shaders/TreeFragment.glsl");
		oasisMap = new HeightMap("../../Textures/oasis.raw");
		oasisMap->SetTexture(SOIL_load_OGL_texture("../../Textures/sand.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 0);
		oasisMap->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"desertbump.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
		
		
		if (!treeShader->LinkProgram()) {
			return;
		}
		if (!oasisMap->GetTexture(0) || !oasisMap->GetBumpMap()) {
			return;
		}
		
		SetTextureRepeating(oasisMap->GetTexture(0), true);
		SetTextureRepeating(quad->GetTexture(0), true);
		SetTextureRepeating(oasisMap->GetBumpMap(), true);
		root3 = new SceneNode();
		for (int i = 0; i < 240; i += 10) {
			
			Tree * tree = new Tree(30, rand()%200+300);
			tree->SetTransform(Matrix4::Translation(Vector3(21000 + 2000 * cos(i/ 57.3) + rand()%500 - 250, 100, 22000 + 2000 * sin(i/57.3) + rand() % 500 - 250)));
			root3->AddChild(tree);
		}
		for (int i = 0; i < 240; i += 5) {

			Plant * plant = new Plant(rand() % 10 + 5);
			plant->SetTransform(Matrix4::Translation(Vector3(21000 + 2000 * cos(i / 57.3) + rand() % 500 - 250, 100, 22000 + 2000 * sin(i / 57.3) + rand() % 500 - 250)));
			root3->AddChild(plant);
		}
		waterRotate = 0.0f;
	}
	if (debug & 0b100)
		scene_fill(2);
	else if (debug & 0b001)
		scene_fill(0);
	else
		scene_fill(1);

	textShader = new Shader(SHADERDIR"TexturedVertex.glsl", SHADERDIR"TexturedFragment.glsl");
	if (!textShader->LinkProgram()) {
		return;
	}
	basicFont = new Font(SOIL_load_OGL_texture(TEXTUREDIR"tahoma.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT), 16, 16);

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	init = true;
}

// Deletes all the heap stuff
Renderer ::~Renderer(void) {
	
	delete quad;
	delete reflectShader;
	delete skyboxShader;
	delete lightShader;
	delete light;
	delete basicFont;
	delete shadowShader;
	if (debug & 0b001) {
		delete heightMap;
		delete root;
		delete pyramidShader;
		Pyramid::DeletePyramid(); // Also important !
		Beam::DeleteBeam();
	}
	if (debug & 0b010) {
		delete root2;
		delete wallShader;
		Room::DeleteRoom(); // Also important !
	}
	else if (debug & 0b100) {
		delete oasisMap;
		delete root3;
		delete treeShader;
		delete oasisShader;
		Tree::DeleteTree(); // Also important !
		
	}
	delete camera;
}


void Renderer::UpdateScene(float msec) {
	
	camera -> UpdateCamera(msec);
	viewMatrix = camera -> BuildViewMatrix();
	if (scene == 0) {
		if (redh>0 && finished())
			UpdateRedh(msec);
		root->Update(msec);
	}
	else if (scene == 1) {
		root2->Update(msec);
	}
	else if (scene == 2) {
		root3->Update(msec);
		waterRotate += msec / 1000.0f;
	}
}

// For scenes 0 and 2, shadows are needed. For scene 1, shadows are not needed.
void Renderer::RenderScene() {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	
	if (scene == 0 || scene == 2) {

		DrawShadowScene(); // First render pass ...
		DrawCombinedScene(); // Second render pass ...		

	}
	else if (scene == 1) {
		SetCurrentShader(wallShader);
		glUseProgram(wallShader->GetProgram());
		SetShaderLight(torchlight);
		UpdateShaderMatrices();
		glUniform3fv(glGetUniformLocation(wallShader->GetProgram(), "cameraPos"), 1, (float *)& camera->GetPosition());
		glUniform1i(glGetUniformLocation(wallShader->GetProgram(), "roomTex1"), 0);
		glUniform1i(glGetUniformLocation(wallShader->GetProgram(), "roomTex2"), 1);
		glUniform1i(glGetUniformLocation(wallShader->GetProgram(), "roomTex3"), 2);
		DrawNode(root2);
	}
	

	SetCurrentShader(textShader);
	glUseProgram(textShader->GetProgram());

	glUniform1i(glGetUniformLocation(textShader->GetProgram(), "textTex"), 0);

	DrawText(fps, Vector3(0, 0, 0), 16.0f);

	glUseProgram(0);
	SwapBuffers();
}

// It binds the correct uniforms for drawing the appropriate scenes
void Renderer::DrawNode(SceneNode * n) {
	if (n -> GetMesh()) {
		if (scene == 0) {
			Matrix4 transform = n->GetWorldTransform()*Matrix4::Scale(n->GetModelScale());
			glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, (float *)& transform);
			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "useTexture"), (int)n->GetMesh()->GetTexture(0));
			glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "redh"), redh);
			n->Draw(*this);
		}
		else if (scene == 1) {
			Matrix4 transform = n->GetWorldTransform()*Matrix4::Scale(n->GetModelScale());
			glUniformMatrix4fv(glGetUniformLocation(wallShader->GetProgram(), "modelMatrix"), 1, false, (float *)& transform);
			glUniformMatrix4fv(glGetUniformLocation(wallShader->GetProgram(), "textureMatrix"), 1, false, (float *)& n->GetTextureMatrix());
			glUniform1i(glGetUniformLocation(wallShader->GetProgram(), "useTexture"), (int)n->GetMesh()->GetTexture(0));
			glUniform1i(glGetUniformLocation(wallShader->GetProgram(), "side"), n->side);
			n->Draw(*this);
		}
		else if (scene == 2) {
			Matrix4 transform = n->GetWorldTransform()*Matrix4::Scale(n->GetModelScale());
			glUniformMatrix4fv(glGetUniformLocation(treeShader->GetProgram(), "modelMatrix"), 1, false, (float *)& transform);
			glUniform4fv(glGetUniformLocation(treeShader->GetProgram(), "nodeColour"), 1, (float *)& n->GetColour());
			glUniform1i(glGetUniformLocation(treeShader->GetProgram(), "useTexture"), (int)n->GetMesh()->GetTexture(0));
			n->Draw(*this);
		}
	}

	for (vector < SceneNode * >::const_iterator i = n -> GetChildIteratorStart(); i != n -> GetChildIteratorEnd(); ++i) {
		DrawNode(*i);
	}
}

// This is a simple drawing method, useful for calculating the shadows.
void Renderer::ShadowNode(SceneNode * n) {
	if (n->GetMesh()) {
		modelMatrix.ToIdentity();

		Matrix4 tempMatrix = textureMatrix * modelMatrix;

		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "textureMatrix"), 1, false, *& tempMatrix.values);

		Matrix4 transform = n->GetWorldTransform()*Matrix4::Scale(n->GetModelScale());
		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, (float *)& transform);

		n->Draw(*this);
	}

	for (vector < SceneNode * >::const_iterator i = n->GetChildIteratorStart(); i != n->GetChildIteratorEnd(); ++i) {
		ShadowNode(*i);
	}
}

// Redh is the height of the color change that happens on the pyramid
void Renderer::UpdateRedh(float msec) {
	float velocity = msec/10000;
	redh = redh - velocity;
}

// This is the method for changing the scene that is being drawn.
void Renderer::scene_fill(int s) {
	scene = s;
	if (s == 0) {
		glDisable(GL_CULL_FACE);
		camera->SetPosition(Vector3(12000, 2000, 10000));
		camera->SetPitch(59);
		camera->SetYaw(207);
	}
	else if (s == 1) {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		camera->SetPosition(Vector3(-5000, 0, -1000));
		camera->SetPitch(0);
		camera->SetYaw(-90);
	}
	else if (s == 2) {
		glDisable(GL_CULL_FACE);
		camera->SetPosition(Vector3(17000, 4000, 17000));
		camera->SetPitch(-30);
		camera->SetYaw(90);
	}
}

// Goes to the next point on the circle with centre of x,y,z and by speed.
void Renderer::ToggleCircle(float x, float y, float z, float speed) {
	camera->SetPosition(Matrix4::Translation(Vector3(x,y,z))*Matrix4::Rotation(speed, Vector3(0, 1, 0))*Matrix4::Translation(Vector3(-x, -y, -z))*camera->GetPosition());
}

// Calculates the yaw for looking at the point x,y,z
float Renderer::GetYaw(float x, float y, float z) {
	float yaw;
	float tx = (camera->GetPosition()).x;
	float tz = (camera->GetPosition()).z;
	float tempx = (tx - x) / sqrt((x - tx)*(x - tx) + (z - tz)*(z - tz));
	float tempz = (tz - z) / sqrt((x - tx)*(x - tx) + (z - tz)*(z - tz));
	float rad = acos(tempx);
	if (tempz < 0)
		rad = -rad;
	yaw = -rad*57.29 + 90;

	if (yaw < 0) {
		yaw += 360.0f;
	}
	if (yaw > 360.0f) {
		yaw -= 360.0f;
	}
	return yaw;
}

// Calculates the pitch for looking at the point x,y,z
float Renderer::GetPitch(float x, float y, float z) {
	float pitch;
	float tx = (camera->GetPosition()).x;
	float ty = (camera->GetPosition()).y;
	float tz = (camera->GetPosition()).z;
	float tempx = (tx - x) / sqrt((x - tx)*(x - tx) + (y - ty)*(y - ty) + (z - tz)*(z - tz));
	float tempy = (ty - y) / sqrt((x - tx)*(x - tx) + (y - ty)*(y - ty) + (z - tz)*(z - tz));
	float rad = asin(tempy);
	
	rad = -rad;
	pitch = rad*57.29;	
	return pitch;
}

// Sets the camera yaw
bool Renderer::SetYaw(float yaw, float speed) {
	bool out = false;
	if (speed) {
		if (abs(yaw - camera->GetYaw())>abs(speed)) {
			yaw = camera->GetYaw() + speed;
			if (yaw < 0) {
				yaw += 360.0f;
			}
			if (yaw > 360.0f) {
				yaw -= 360.0f;
			}
			out = true;
		}
	}
	camera->SetYaw(yaw);
	return out;
}

// Sets the camera pitch
bool Renderer::SetPitch(float pitch, float speed) {
	bool out = false;
	if (speed) {
		if (abs(pitch - camera->GetPitch())>abs(speed)) {
			if (pitch - camera->GetPitch()<0)
				pitch = camera->GetPitch() - abs(speed);
			else 
				pitch = camera->GetPitch() + abs(speed);
			out = true;
		}
	}
	
	camera->SetPitch(pitch);
	return out;
}

void Renderer::DrawSkybox() {
	glDepthMask(GL_FALSE);
	SetCurrentShader(skyboxShader);

	UpdateShaderMatrices();
	quad->Draw();

	glUseProgram(0);
	glDepthMask(GL_TRUE);
}

void Renderer::DrawHeightmap(HeightMap * HM, int pass) {
	
	if (pass == 1)
		SetShaderLight(*light);

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float *)& camera->GetPosition());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "bumpTex"), 3);

	modelMatrix.ToIdentity();
	textureMatrix.ToIdentity();

	UpdateShaderMatrices();

	HM->Draw();
	glUseProgram(0);
}

void Renderer::DrawWater() {
	SetCurrentShader(reflectShader);
	SetShaderLight(*light);
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float *)& camera->GetPosition());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "cubeTex"), 4);


	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

	float heightX = 2000;
	float heightY = 60;

	float heightZ = 2000;

	Matrix4 MM = modelMatrix;
	Matrix4 TM = textureMatrix;
	modelMatrix =
		Matrix4::Translation(Vector3(21000, heightY, 22000)) *
		Matrix4::Scale(Vector3(heightX, 1, heightZ)) *
		Matrix4::Rotation(90, Vector3(1.0f, 0.0f, 0.0f));

	textureMatrix = Matrix4::Scale(Vector3(10.0f, 10.0f, 10.0f)) * Matrix4::Rotation(waterRotate, Vector3(0.0f, 0.0f, 1.0f));

	UpdateShaderMatrices();

	quad->Draw();

	modelMatrix = MM;
	textureMatrix = TM;

	glUseProgram(0);
}

void Renderer::DrawText(const std::string &text, const Vector3 &position, const float size) {
	//Create a new temporary TextMesh, using our line of text and our font
	TextMesh* mesh = new TextMesh(text, *basicFont);

	
	Matrix4 MM = modelMatrix;
	Matrix4 VM = viewMatrix;
	Matrix4 PM = projMatrix;
	modelMatrix = Matrix4::Translation(Vector3(position.x, height - position.y, position.z)) * Matrix4::Scale(Vector3(size, size, 1));
	viewMatrix.ToIdentity();
	projMatrix = Matrix4::Orthographic(-1.0f, 1.0f, (float)width, 0.0f, (float)height, 0.0f);

	//Either way, we update the matrices, and draw the mesh
	UpdateShaderMatrices();
	mesh->Draw();

	modelMatrix = MM;
	viewMatrix = VM;
	projMatrix = PM;
	delete mesh; //Once it's drawn, we don't need it anymore!
}

// Hardcoded directional torchlights
void Renderer::setTorchlight() {
	Light * l1 = new Light(Vector3(0, 160, 500), Vector4(0.9f, 0.9f, 1.0f, 1), 2000); //-z
	l1->SetNormal(Vector3(0, 0, 1));
	(torchlight).push_back(l1);
	Light * l2 = new Light(Vector3(0, 160, -2500), Vector4(0.9f, 0.9f, 1.0f, 1), 2000); //-z
	l2->SetNormal(Vector3(0, 0, -1));
	(torchlight).push_back(l2);
	
	Light * l3 = new Light(Vector3(-5500, 160, -995), Vector4(0.9f, 0.9f, 1.0f, 1), 2000); //-x
	l3->SetNormal(Vector3(-1, 0, 0));
	(torchlight).push_back(l3);


	Light * l4 = new Light(Vector3(6000, 160, 500), Vector4(0.9f, 0.9f, 1.0f, 1), 2000); //-z
	l4->SetNormal(Vector3(0, 0, 1));
	(torchlight).push_back(l4);
	
	Light * l5 = new Light(Vector3(7500, 160, -1000), Vector4(0.9f, 0.9f, 1.0f, 1), 2000); //-x
	l5->SetNormal(Vector3(1, 0, 0));
	(torchlight).push_back(l5);
	

	Light * l6 = new Light(Vector3(7500, 160, -7000), Vector4(0.9f, 0.9f, 1.0f, 1), 2000); //-x
	l6->SetNormal(Vector3(1, 0, 0));
	(torchlight).push_back(l6);
	Light * l7 = new Light(Vector3(4500, 160, -7000), Vector4(0.9f, 0.9f, 1.0f, 1), 2000); //-x
	l7->SetNormal(Vector3(-1, 0, 0));
	(torchlight).push_back(l7);


	Light * l8 = new Light(Vector3(4500, 160, -13000), Vector4(0.9f, 0.9f, 1.0f, 1), 2000); //-x
	l8->SetNormal(Vector3(-1, 0, 0));
	(torchlight).push_back(l8);
	Light * l9 = new Light(Vector3(6000, 160, -14500), Vector4(0.9f, 0.9f, 1.0f, 1), 2000); //-z
	l9->SetNormal(Vector3(0, 0, -1));
	(torchlight).push_back(l9);

	Light * l10 = new Light(Vector3(15000, 160, -11500), Vector4(0.9f, 0.9f, 1.0f, 1), 2000); //-z
	l10->SetNormal(Vector3(0, 0, 1));
	(torchlight).push_back(l10);
	Light * l11 = new Light(Vector3(15000, 160, -14500), Vector4(0.9f, 0.9f, 1.0f, 1), 2000); //-z
	l11->SetNormal(Vector3(0, 0, -1));
	(torchlight).push_back(l11);

	Light * l12 = new Light(Vector3(31000, 1600, -5000), Vector4(0.9f, 0.9f, 1.0f, 1), 10000); //-z
	l12->SetNormal(Vector3(0, 0, 1));
	(torchlight).push_back(l12);
	Light * l13 = new Light(Vector3(31000, 1600, -23000), Vector4(0.9f, 0.9f, 1.0f, 1), 10000); //-z
	l13->SetNormal(Vector3(0, 0, -1));
	(torchlight).push_back(l13);
	Light * l14 = new Light(Vector3(40000, 1600, -14000), Vector4(0.9f, 0.9f, 1.0f, 1), 10000); //-x
	l14->SetNormal(Vector3(1, 0, 0));
	(torchlight).push_back(l14);
	Light * l15 = new Light(Vector3(21000, 1600, -14000), Vector4(0.9f, 0.9f, 1.0f, 1), 10000); //-x
	l15->SetNormal(Vector3(-1, 0, 0));
	(torchlight).push_back(l15);
}

// Disco effect. Randomly changes color.
void Renderer::disco() {
	for (int i=11; i<15; i++)
		torchlight[i]->SetColour(Vector4(float(rand()%100) /100, float(rand() % 100) / 100, float(rand() % 100) / 100, 1));
}


void Renderer::DrawShadowScene() {
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);

	glClear(GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	SetCurrentShader(shadowShader);

	viewMatrix = Matrix4::BuildViewMatrix(light->GetPosition(), Vector3(19500, 0, 19500));
	shadowMatrix = biasMatrix *(projMatrix * viewMatrix);

	UpdateShaderMatrices();

	if (scene == 0) {

		//DrawHeightmap(heightMap,0);
		ShadowNode(root);


	}
	else if (scene == 2) {

		//DrawHeightmap(oasisMap,0);
		ShadowNode(root3);
	}

	glUseProgram(0);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glViewport(0, 0, width, height);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Renderer::DrawCombinedScene() {
	SetCurrentShader(lightShader);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "bumpTex"), 3);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "shadowTex"), 4);

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float *)& camera->GetPosition());

	SetShaderLight(*light);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, shadowTex);

	viewMatrix = camera->BuildViewMatrix();
	UpdateShaderMatrices();

	if (scene == 0) {
		DrawSkybox();
		SetCurrentShader(lightShader);
		DrawHeightmap(heightMap,1);

		SetCurrentShader(pyramidShader);
		SetShaderLight(*light);
		glUseProgram(currentShader->GetProgram());
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
		
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "shadowTex"), 4);

		glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float *)& camera->GetPosition());
		UpdateShaderMatrices();
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "pyramidTex1"), 0);
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "pyramidTex2"), 1);
		DrawNode(root);


	}
	else if (scene == 2) {
		DrawSkybox();
		SetCurrentShader(lightShader);
		DrawHeightmap(oasisMap,1);
		DrawWater();

		SetCurrentShader(treeShader);

		glUseProgram(currentShader->GetProgram());
		UpdateShaderMatrices();

		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
		DrawNode(root3);
	}

	glUseProgram(0);

}