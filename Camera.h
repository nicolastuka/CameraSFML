#ifndef CAMERA_H
#define CAMERA_H

#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;

class Camera : public View {
private:
	RenderWindow *window;
	Vector2f windowDimension;
	Vector2f worldDimension;
	Vector2f cameraPosition;
	float zoomCamera;
	float timeShake;
	int forceShake;
	float timeFlash;
	Sprite sShaderFlash;
	Texture tShaderFlash;
	Shader shader;
	
	void CalculateCameraPosition(Vector2f target) {
		cameraPosition = target;
		if (cameraPosition.x < windowDimension.x / 2) {
			cameraPosition.x = windowDimension.x / 2;
		}
		if (cameraPosition.x > worldDimension.x - windowDimension.x / 2) {
			cameraPosition.x = worldDimension.x - windowDimension.x / 2;
		}
		if (cameraPosition.y < windowDimension.y / 2) {
			cameraPosition.y = windowDimension.y / 2;
		}
		if (cameraPosition.y > worldDimension.y - windowDimension.y / 2) {
			cameraPosition.y = worldDimension.y - windowDimension.y / 2;
		}
	}
	
	void MoveCamera() {
		setCenter(cameraPosition);
	}
	
	void ShakeCamera() {
		if (timeShake > 0) {
			cameraPosition.x += rand() % forceShake;
			cameraPosition.y += rand() % forceShake;
			timeShake -= 0.01f;
			setCenter(cameraPosition);
		}
	}
	
	void FlashCamera() {
		if (timeFlash > 0) {
			shader.setUniform("active", true);
			tShaderFlash.loadFromImage(window->capture());
			sShaderFlash.setTexture(tShaderFlash);
			sShaderFlash.setPosition(getCenter().x - window->getSize().x / 2, getCenter().y - window->getSize().y / 2);
			window->clear(Color::White);
			window->draw(sShaderFlash, &shader);
			window->display();
			timeFlash -= 0.1;
		} else {
			shader.setUniform("active", false);
			timeFlash = 0;
		}
	}
	
public:
	Camera(RenderWindow *w, int width, int height, int worldLimitWidth, int worldLimitHeight) {
		window = w;
		zoomCamera = 1.f;
		timeShake = 0;
		timeFlash = 0;
		windowDimension.x = width;
		windowDimension.y = height;
		worldDimension.x = worldLimitWidth;
		worldDimension.y = worldLimitHeight;
		setSize(width, height);
		setCenter(worldLimitWidth / 2, worldLimitHeight / 2);
		shader.loadFromFile("assets/shaders/FlashShader.frag", sf::Shader::Fragment);
		shader.setUniform("flashColor", sf::Glsl::Vec4(1, 1, 1, 1));
	}
	
	void FollowAndUpdate(Vector2f target, Camera *camera) {
		CalculateCameraPosition(target);
		MoveCamera();
		ShakeCamera();
		FlashCamera();
		window->setView(*camera);
	}
	
	void Rotate(float value) {
		setRotation(getRotation() + value);
	}
	
	void SetZoom(float factor) {
		setSize(windowDimension);
		zoomCamera += factor;
		if (zoomCamera < 0) {
			zoomCamera -= factor;
		}
		zoom(zoomCamera);
	}
	
	void Shake(float time, int force) {
		timeShake = time;
		forceShake = force;
	}
	
	void Flash(float time) {
		timeFlash = time;
	}
};
#endif
