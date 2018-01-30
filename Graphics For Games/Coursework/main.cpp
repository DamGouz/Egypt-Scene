#include "../../nclgl/window.h"
#include "Renderer.h"

#pragma comment(lib, "nclgl.lib")

int main() {
	int scene = 0;
	bool started = false;
	bool cinematic = false;
	int mult_count = 0;
	int step = 0;
	float yaw = 0;
	float pitch = 0;
	float speed = 0.2;
	bool action = false;
	float cnt = 0;
	Window w("Ancient Egypt", 1920, 1080, true);
	if (!w.HasInitialised()) {
		return -1;
	}
	//Renderer2 renderer_inside(w);
	Renderer renderer(w);
	
	if (!renderer.HasInitialised()) {
		return -1;
	}

	w.LockMouseToWindow(true);
	w.ShowOSPointer(false);

	while (w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) {
		renderer.setfps(speed);
		speed = speed / 40;
		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_C)) {
			cinematic = !cinematic;
			action = true;
		}

		if (!cinematic) {
			if (Window::GetKeyboard()->KeyDown(KEYBOARD_R)) {
				renderer.ToggleCircle(14500, 0 , 14500, 0.1);
				renderer.SetYaw(renderer.GetYaw(14500, 0, 14500), 0);
				renderer.SetPitch(renderer.GetPitch(14500, 0, 14500), 0);
			}
			else if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_RIGHT)) {
				int x = renderer.getScene();
				action = true;
				if (x == 0) {
					renderer.scene_fill(1);
					step = 7;
				}
				else if (x==1) {
					renderer.scene_fill(2);
					step = 0;
				}
				else if (x == 2) {
					renderer.scene_fill(0);
					step = 3;
				}
			}
			else if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_LEFT)) {
				int x = renderer.getScene();
				action = true;
				if (x == 2) {
					renderer.scene_fill(1);
					step = 7;
				}
				else if (x == 0) {
					renderer.scene_fill(2);
					step = 0;
				}
				else if (x == 1) {
					renderer.scene_fill(0);
					step = 3;
				}
			}
		}
		// The cinematic starts here. 15 steps to do the whole round.
		else {
			if (action) {
				if (step == 0) {
					yaw = renderer.GetYaw(21000, 0, 22000);
					pitch = renderer.GetPitch(21000, 0, 22000);
				}
				else if (step == 1) {
					renderer.startTrees();
				}
				else if (step == 2) {
					yaw = 207;
					pitch = 59;
				}
				else if (step == 3) {
					renderer.scene_fill(0);
					renderer.startBeams();
				}
				else if (step == 4) {
					yaw = renderer.GetYaw(14500, 0, 14500);
					pitch = renderer.GetPitch(14500, 0, 14500);
				}
				else if (step == 5) {
					//renderer.SetPitch(renderer.GetPitch(14500, 0, 14500),0);
				}
				else if (step == 6) {
					renderer.gotoTarget(15000, 200, 15000, 100/speed);
				}
				else if (step == 7) {
					renderer.scene_fill(1);
				}
				else if (step == 8) {
					renderer.gotoTarget(6000, 0, -1000, 100 / speed);
				}
				else if (step == 9) {
					yaw = renderer.GetYaw(6000, 0, -2000);
					pitch = renderer.GetPitch(6000, 0, -2000);
				}
				else if (step == 10) {
					renderer.gotoTarget(6000, 0, -13000, 100 / speed);
				}
				else if (step == 11) {
					yaw = renderer.GetYaw(7000, 0, -13000);
					pitch = renderer.GetPitch(7000, 0, -13000);
				}
				else if (step == 12) {
					renderer.gotoTarget(21000, 0, -13000, 100 / speed);
				}
				else if (step == 13) {
					renderer.gotoTarget(37000, 1500, -13000, 100 / speed);
				}
				else if (step == 14) {
					yaw = renderer.GetYaw(30000, 2000, -14000);
					pitch = renderer.GetPitch(30000, 2000, -14000);
				}
				else if (step == 15) {
				}
				action = false;
			}
			else {
				if (step == 0) {
					if (!renderer.SetYaw(yaw, speed) && !renderer.SetPitch(pitch, speed)) {
						step = 1;
						action = true;
					}
				}
				else if (step == 1) {
					if (!renderer.finishedTrees()) {
						
						renderer.ToggleCircle(21000, 0, 22000, speed);
						renderer.SetYaw(renderer.GetYaw(21000, 0, 22000), 0);
					}
					else {
						
						step++;
						action = true;
					}
				}
				else if (step == 2) {
					if (!renderer.SetPitch(pitch, speed) && !renderer.SetYaw(yaw, speed)) {
						step ++;
						action = true;
					}
				}
				else if (step == 3) {
					if (!renderer.finished()) {
						pitch = renderer.GetPitch(14500, renderer.followbeam(), 14500);
						renderer.SetPitch(pitch, 0);
					}
					else {
						step++;
						action = true;
					}
				}
				else if (step == 4) {
					if (!renderer.SetYaw(yaw, speed) && !renderer.SetPitch(pitch, speed)) {
						step ++;
						action = true;
					}
				}
				else if (step == 5) {
					if (renderer.getredh()>0) {
						
						renderer.ToggleCircle(14500, 0, 14500, speed);
						renderer.SetYaw(renderer.GetYaw(14500, 0, 14500),0);
					}
					else {
						
						step++;
						action = true;
					}
				}
				else if (step == 6) {
					if (renderer.there_yet()) {
						step++;
						action = true;
					}
				}
				else if (step == 7) {
					step++;
					action = true;
				}
				else if (step == 8) {
					if (renderer.there_yet()) {
						step++;
						action = true;
					}
				}
				else if (step == 9) {
					if (!renderer.SetYaw(yaw, speed * 2)&&!renderer.SetPitch(pitch, speed * 2)) {
						step++;
						action = true;
					}
				}
				else if (step == 10) {
					if (renderer.there_yet()) {
						step++;
						action = true;
					}
				}
				else if (step == 11) {
					if (!renderer.SetYaw(yaw, -speed * 2) && !renderer.SetPitch(pitch, speed * 2)) {
						step++;
						action = true;
					}
				}
				else if (step == 12) {
					if (renderer.there_yet()) {
						step++;
						action = true;
					}
				}
				else if (step == 13) {
					if (renderer.there_yet()) {
						step++;
						action = true;
					}
				}
				else if (step == 14) {
					if (!renderer.SetYaw(yaw, -speed*2) && !renderer.SetPitch(pitch, speed * 2)) {
						step++;
						action = true;
					}
				}
				else if (step == 15) {
					renderer.ToggleCircle(30000, 2000, -14000, speed);
					renderer.SetYaw(renderer.GetYaw(30000, 2000, -14000), 0);

					cnt += speed;
					if (cnt > 20) {
						cnt = 0;
						renderer.disco();
					}
				}
			}
		}
		speed = w.GetTimer()->GetTimedMS();
		renderer.UpdateScene(speed);
		renderer.RenderScene();
		
	}

	return 0;
}