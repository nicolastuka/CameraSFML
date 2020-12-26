#include <SFML/Graphics.hpp>
#include "Camera.h"

using namespace sf;

int main(int argc, char *argv[]){
	RenderWindow w(VideoMode(320,240),"Ejemplo de Camera SFML");
	w.setFramerateLimit(60);
	Texture t;
	Sprite s;
	
	Texture tMosca;
	tMosca.loadFromFile("assets/textures/mosca.png");
	Sprite sMosca;
	sMosca.setTexture(tMosca);
	sMosca.setOrigin(tMosca.getSize().x / 2, tMosca.getSize().y / 2);
	
	t.loadFromFile("assets/textures/foto.jpg");
	s.setTexture(t);
	s.setPosition(0, 0);
	Camera camera(&w, 320, 240, t.getSize().x, t.getSize().y);
	
	while(w.isOpen()) {
		Event e;
		while(w.pollEvent(e)) {
			if(e.type == Event::Closed)
				w.close();
		}
		if (Mouse::isButtonPressed(Mouse::Left)) {
			camera.Rotate(-0.1);
		}
		if (Mouse::isButtonPressed(Mouse::Right)) {
			camera.Rotate(0.1);
		}
		if (Keyboard::isKeyPressed(Keyboard::Z)) {
			camera.SetZoom(0.1f);
		}
		if (Keyboard::isKeyPressed(Keyboard::A)) {
			camera.SetZoom(-0.1f);			
		}
		
		if (Keyboard::isKeyPressed(Keyboard::S)) {
			camera.Shake(1.5f, 150);
		}
		
		if (Keyboard::isKeyPressed(Keyboard::F)) {
			camera.Flash(.5f);
		}
		
		if (Keyboard::isKeyPressed(Keyboard::Left)) {
			sMosca.move(-5, 0);
		}
		if (Keyboard::isKeyPressed(Keyboard::Right)) {
			sMosca.move(5, 0);
		}
		if (Keyboard::isKeyPressed(Keyboard::Up)) {
			sMosca.move(0, -5);
		}
		if (Keyboard::isKeyPressed(Keyboard::Down)) {
			sMosca.move(0, 5);
		}
	
		camera.FollowAndUpdate(sMosca.getPosition(), &camera);
		
		w.clear(Color(255,255,255,255));
		w.draw(s);
		w.draw(sMosca);
		w.display();
	}
	return 0;
}

