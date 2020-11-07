#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <math.h>

int main()
{
	sf::RenderWindow window(sf::VideoMode(1920, 1020), "plotr", sf::Style::Default);
	window.setPosition(sf::Vector2i(0,0));

	// Declaring important variables
	int x_size = window.getSize().x, y_size = window.getSize().y; // Defining size of the window, customizable
	float zoom_factor = 1.0f; // This one changes whenever user zooms in / out
	float raw_x, raw_y; // These will be used for the plotting of the functions themselves

	// Function variables (form: f(x) = ax(^n) + bx(^n-1) ... + cx^0
	// testing purposes, later this will be input by the user
	float a = 4, b = 3, n = 5, c = 2;

	// Creating the circle to indicate which point we are at
	sf::CircleShape pt(3.f);
	pt.setFillColor(sf::Color::Red);
	// Aesthetic purposes, just the triangle at the extremities of our coord. system
	sf::CircleShape tri(8.f);
	tri.setPointCount(3);
	tri.setFillColor(sf::Color::Black);

	// Creating the pixel which will create the coordinate system 
	sf::RectangleShape pixel(sf::Vector2f(2.f, 2.f));
	pixel.setFillColor(sf::Color::Black);


	// This point is used for drawing the function
	sf::RectangleShape f_pixel(sf::Vector2f(4.f, 4.f));
	f_pixel.setFillColor(sf::Color::Red);

	

	while (window.isOpen()) 
	{
		sf::Event event;
		while (window.pollEvent(event)) 
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::Resized)
				window.setSize(sf::Vector2u(event.size.width, event.size.height));
			else if (event.type == sf::Event::MouseWheelScrolled)
			{
				
				// Deciding whether user wants to zoom in or out
				// Fix with a stack
				if (event.mouseWheelScroll.delta > 0) // zoom in
					zoom_factor -= 0.25f;
				/* Explanation: This is a zoom in because what we effectively do is only make the distance between
				the points smaller so we can fit more points in the screen. As our zoom factor is only used in
				calculating the distance between points, we can make it smaller by multiplying it with 0.75,
				which seems counter-intuitive but works!! Exactly because we have to fit more points, i.e to get
				more accurate numbers. Zooming out would mean getting less accurate, but more numbers, which is
				why distances get bigger. Also, a stack is used to guarantee that the zooming works properly.*/
				else // zoom out
					zoom_factor += 0.25f;
			}
		}

		window.clear(sf::Color(237,237,237)); // Clearing window with grey color

		// Drawing coordinate system, first x-axis (here) then y-axis (line 89)
		for (int x = 0; x < window.getSize().x; x += 2)
		{
			pixel.setPosition(sf::Vector2f(x, (y_size-60) / 2));
			window.draw(pixel);
			if (x % (int)(160 * zoom_factor) == 0)
			{
				pt.setPosition(pixel.getPosition()); // 1 length unit = 160 pixels
				window.draw(pt);
			}
			if (x == (window.getSize().x - 2))
			{
				tri.setRotation(90); // These are the arrows
				tri.setPosition(sf::Vector2f(x, pixel.getPosition().y - 7));
				window.draw(tri);
			}

		}

		for (int y = 0; y <= window.getSize().y-60; y += 2)
		{
			pixel.setPosition(sf::Vector2f(x_size / 2, y));
			window.draw(pixel);
			if (y % (int)(160 * zoom_factor) == 0)
			{
				pt.setPosition(pixel.getPosition());
				window.draw(pt);
			}
			if (y == 0)
			{
				tri.setRotation(0);
				tri.setPosition(sf::Vector2f(pixel.getPosition().x - 7, y));
				window.draw(tri);
			}
		}
		
		if (a != 0) // We only draw if our function isn't equal to one of the axes
		{
			for (float x = 0; x < window.getSize().x; x+=0.05f)
			{
				raw_x = -6.f + (x / 160.f);
				raw_y = a * pow(raw_x,n) + b*pow(raw_x,n-1) + c;
				if (raw_y > 0) f_pixel.setPosition(x, (3 - raw_y) * 160.f);
				else f_pixel.setPosition(x, 960 - (3 + raw_y) * 160.f);

				window.draw(f_pixel);
			}
		}
		window.display();
	}

	return 0;
}