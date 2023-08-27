#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <memory>

class DShape
{
public:
	std::shared_ptr<sf::Shape> shape;
	float x = 0.0f, y = 0.0f;
	float speedX = 0.0f, speedY = 0.0f;
	sf::Text text;

	DShape(float x, float y, float speedX, float speedY)
		: x(x) ,y(y)
		, speedX(speedX) , speedY(speedY)
	{
	}
};

int main()
{
	// List of all shapes
	std::vector<DShape> shapes;

	// Load in from config file
	std::ifstream fin("config.txt");
	std::string configType;

	// Get window parameters from the config file
	int wWidth = 400;
	int wHeight = 400;
	if (fin >> configType)
	{
		fin >> wWidth >> wHeight;
	}
	else
	{
		std::cerr << "Could not load text from config.txt" << std::endl;
		exit(-1);
	}

	// Set up window with parameters
	sf::RenderWindow window(sf::VideoMode(wWidth, wHeight), "My Window");
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);

	// Get text parameters from config file
	std::string fontFilePath;
	int fontSize = 0, fontR = 0, fontG = 0, fontB = 0;
	if (fin >> configType)
	{
		fin >> fontFilePath >> fontSize >> fontR >> fontG >> fontB;
	}

	// Apply parameters to font
	sf::Font font;
	if (!font.loadFromFile(fontFilePath))
	{
		std::cerr << "Could not load font from file" << std::endl;
		exit(-1);
	}
	sf::Color fontColor = sf::Color(fontR, fontG, fontB);

	// Load in all the shapes from the config file
	while (fin >> configType)
	{
		std::string name = "";
		float x = 0.0f, y = 0.0f;
		float speedX = 0.0f, speedY = 0.0f;
		int r = 0, g = 0, b = 0;

		if (configType == "Rectangle")
		{
			float width = 0.0f, height = 0.0f;
			fin >> name >> x >> y >> speedX >> speedY >> r >> g >> b >> width >> height;

			DShape rectangle(x, y, speedX, speedY);
			rectangle.shape = std::make_shared<sf::RectangleShape>(sf::Vector2f(width, height));
			rectangle.shape->setFillColor(sf::Color(r, g, b));

			// initialize text and set its color
			sf::Text text = sf::Text(name, font, fontSize);
			text.setFillColor(fontColor);
			// move the origin of the text to the center to make my life easier
			sf::FloatRect textBounds = text.getLocalBounds();
			text.setOrigin(sf::Vector2f(textBounds.left + textBounds.width / 2,
				textBounds.top + textBounds.height / 2));

			rectangle.text = text;
			shapes.push_back(rectangle);
		}

		else if (configType == "Circle")
		{
			float radius;
			fin >> name >> x >> y >> speedX >> speedY >> r >> g >> b >> radius;

			DShape circle(x, y, speedX, speedY);
			circle.shape = std::make_shared<sf::CircleShape>(radius);
			circle.shape->setFillColor(sf::Color(r, g, b));
			
			// initialize text and set its color
			sf::Text text = sf::Text(name, font, fontSize);
			text.setFillColor(fontColor);
			// move the origin of the text to the center to make my life easier
			sf::FloatRect textBounds = text.getLocalBounds();
			text.setOrigin(sf::Vector2f(textBounds.left + textBounds.width / 2,
							textBounds.top + textBounds.height / 2));

			circle.text = text;
			shapes.push_back(circle);
		}
	}

	// run the program as long as the window is open
	while (window.isOpen())
	{
		// check all the window's events that were triggered
		// since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
			{
				window.close();
				return 0;
			}
		}

		// update shapes and text
		for (auto& shape : shapes)
		{
			sf::FloatRect bounds = shape.shape->getLocalBounds();

			// move
			shape.x += shape.speedX;
			shape.y += shape.speedY;

			// bounce
			if (shape.x < 0 || shape.x + bounds.width > wWidth)
			{
				shape.speedX *= -1;
			}
			if (shape.y < 0 || shape.y + bounds.height > wHeight)
			{
				shape.speedY *= -1;
			}

			shape.shape->setPosition(sf::Vector2f(shape.x, shape.y));

			// update text position after moving the shape
			shape.text.setPosition(sf::Vector2f(shape.x + bounds.width / 2, shape.y + bounds.height / 2));

		}

		// clear the window with black color
		window.clear(sf::Color::Black);

		// draw everything here
		for (auto& shape : shapes)
		{
			window.draw(*shape.shape);
			window.draw(shape.text);
		}

		// end the current frame
		window.display();
	}

	return 0;
}