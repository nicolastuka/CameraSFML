/**
* @file Camera.h
* @version 0.5
* @date 12/28/2020
* @author Nicolas Capel
* @brief Clase Camera para simular proyecciones de escenas con SFML 2.5. Hereda de View.
* @code
* //Ejemplo de uso en archivo main.cpp
* @endcode
*/

#ifndef CAMERA_H
#define CAMERA_H

#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;

/**
* @brief Clase que se encarga de manipular la visualizaci�n de escena, hereda de View por ende incorpora todos sus m�todos y propiedades
*/

class Camera : public View {
private:
	
	/**
	* @brief Propiedad puntero a la ventana que aplica la View
	*/
	RenderWindow *window;
	
	/**
	* @brief Propiedad para recordar las dimensiones de la ventana
	*/
	Vector2f windowDimension;
	
	/**
	* @brief Propiedad para recordar el tama�o del mundo o escena m�s all� de los l�mites de la ventana
	*/
	Vector2f worldDimension;
	
	/**
	* @brief Posici�n a la que enfoca la c�mara en la escena
	*/
	Vector2f cameraPosition;
	
	/**
	* @brief Propiedad para recordar el zoom establecido por el usuario
	*/
	float zoomCamera;
	
	/**
	* @brief Propiedad para recordar el tiempo de duraci�n del efecto shake
	*/
	float timeShake;
	
	/**
	* @brief Propiedad para recordar la fuerza de shake a establecer en el efecto
	*/
	int forceShake;
	
	/**
	* @brief Propiedad para recordar el tiempo que debe durar el efecto flash
	*/
	float timeFlash;
	
	/**
	* @brief Propiedad Sprite para crear el efecto de flash
	*/
	Sprite sShaderFlash;
	
	/**
	* @brief Propiedad Texture para crear el efecto de flash
	*/
	Texture tShaderFlash;
	
	/**
	* @brief Shader que se aplica al Sprite sShaderFlash
	*/
	Shader shader;
	
	/**
	* @brief Calcula la posici�n de la c�mara seg�n la posici�n del objetivo.
	* @param target Punto X e Y de la posici�n del elemento seguido
	*/
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
	
	/**
	* @brief Centra la c�mara seg�n lo obtenido en el m�todo anterior.
	*/
	void MoveCamera() {
		setCenter(cameraPosition);
	}
	
	/**
	* @brief Ejecuta el efecto Shake.
	*/
	void ShakeCamera() {
		if (timeShake > 0) {
			cameraPosition.x += rand() % forceShake;
			cameraPosition.y += rand() % forceShake;
			timeShake -= 0.01f;
			setCenter(cameraPosition);
		}
	}
	
	/**
	* @brief Ejecuta el efecto flash.
	*/
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
	/**
	* @brief Constructor.
	* @param w Puntero a la ventana donde se aplica la c�mara
	* @param width Ancho de la ventana
	* @param heifht Alto de la ventana
	* @param worldLimitWidth Ancho del mundo o escena
	* @param worldLimitHeight Alto del mundo o escena
	*/
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
	
	/**
	* @brief M�todo que sigue a una posici�n en pantalla y actualizaci�n de c�mara.
	* @param target Posici�n de centro de c�mara
	* @param camera Direcci�n de memoria de la propia instancia (menor c�digo en implementaciones)
	*/
	void FollowAndUpdate(Vector2f target, Camera *camera) {
		CalculateCameraPosition(target);
		MoveCamera();
		ShakeCamera();
		FlashCamera();
		window->setView(*camera);
	}
	
	/**
	* @brief M�todo que establece la rotaci�n de la c�mara de modo acumulativo
	* @param value Valor en flotante que establece el valor de rotaci�n (puede ser negativo)
	*/
	void Rotate(float value) {
		setRotation(getRotation() + value);
	}
	
	/**
	* @brief M�todo que establece el zoom de la c�mara de modo acumulativo
	* @param factor Valor en flotante que establece el valor de zoom (no puede ser negativo)
	*/
	void SetZoom(float factor) {
		setSize(windowDimension);
		zoomCamera += factor;
		if (zoomCamera < 0) {
			zoomCamera -= factor;
		}
		zoom(zoomCamera);
	}
	
	/**
	* @brief M�todo que configura como ejecutar el efecto shake
	* @param time Valor en flotante que establece la duraci�n del mismo
	* @param time Valor en entero que establece la fuerza del mismo
	*/
	void Shake(float time, int force) {
		timeShake = time;
		forceShake = force;
	}
	
	/**
	* @brief M�todo que configura como ejecutar el efecto flash
	* @param time Valor en flotante que establece la duraci�n del mismo
	*/
	void Flash(float time) {
		timeFlash = time;
	}
};
#endif
