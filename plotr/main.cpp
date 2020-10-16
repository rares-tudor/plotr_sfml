#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <queue>

int main()
{
	sf::RenderWindow window(sf::VideoMode(1920, 1020), "plotr", sf::Style::Default);
	window.setPosition(sf::Vector2i(0,0));

	// Declaring important variables
	int x_size = window.getSize().x, y_size = window.getSize().y; // Defining size of the window, customizable
	float zoom_factor = 1.0f; // This one changes whenever user zooms in / out

	// Creating the circle to indicate which point we are at
	sf::CircleShape pt(3.f);
	pt.setFillColor(sf::Color::Red);

	// Creating the pixel which will create the grid, drawing it iteratively on the canvas
	sf::RectangleShape pixel(sf::Vector2f(2.f, 2.f));
	sf::CircleShape tri(8.f);
	tri.setPointCount(3);
	tri.setFillColor(sf::Color::Black);
	pixel.setFillColor(sf::Color::Black);

	// Draws the entire grid, useful later..
	/*for (int x = 0; x < window.getSize().x; x+=2)
	{
		for (int y = 0; y < window.getSize().y; y+=2)
		{
			pixel.setFillColor(sf::Color::White);
			if (y == (window.getSize().y / 2) || x == (window.getSize().x / 2))
			{
				pixel.setFillColor(sf::Color::Black);
				pixel.setPosition(sf::Vector2f(x,y));
				window.draw(pixel);
				window.display();
				continue;
			}
			pixel.setPosition(sf::Vector2f(x, y));
			window.draw(pixel);
			window.display();
		}
	}*/
	sf::VertexArray vertex(sf::Lines, 2);
	vertex[0].position = sf::Vector2f(0.f,0.f);
	vertex[1].position = sf::Vector2f(1920.f, 960.f);
	vertex[0].color = sf::Color::Red;
	vertex[1].color = sf::Color::Red;
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

		// Drawing coordinate system
		for (int x = 0; x < window.getSize().x; x += 2)
		{
			pixel.setPosition(sf::Vector2f(x, (y_size-60) / 2));
			window.draw(pixel);
			if (x % (int)(160 * zoom_factor) == 0)
			{
				pt.setPosition(pixel.getPosition());
				window.draw(pt);
			}
			if (x == (window.getSize().x - 2))
			{
				tri.setRotation(90);
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
		window.draw(vertex);
		window.display();
	}

	return 0;
}