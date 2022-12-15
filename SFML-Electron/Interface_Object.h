#pragma once
#include <SFML/Graphics.hpp>
#include <fstream>
#include <format>
#include <iostream>
#include <string>
#define defaultSizeX 100
#define defaultSizeY 100
using namespace std;
using namespace sf;

extern Font font;

class MainClass
{
public:
	int windowHeight;
	int windowWidth;
	
	Image diodeImage, transistorImage, capacitatorImage, ammeterImage, bulbImage, cellImage, fuseImage, resistorImage, voltmeterImage;
	Texture componentTexture;

	MainClass() = default;
	

};

class Slider
{
public:
	float value = 1;
	float maxValue = 100;
	float minValue = 1;

	Vector2f previousMouseCoords;
	Vector2f currentMouseCoords;

	float minX, maxX;

	Vector2f position;
	Vector2f buttonPosition;

	bool isDraggingButton = false;


	RectangleShape sliderShape;
	CircleShape sliderButton;
	Text sliderText;

	Slider(Vector2f _position)
	{
		buttonPosition = Vector2f(_position.x - 5, _position.y - 5);
		position = _position;
		minX = _position.x - 5;
		maxX = _position.x + 94;
		sliderShape.setPosition(_position);
		sliderShape.setSize(Vector2f(99, 5));
		sliderShape.setFillColor(Color(155, 155, 155, 255));
		sliderButton.setPosition(buttonPosition + Vector2f(0, 5));
		sliderButton.setRadius(3);
		sliderButton.setFillColor(Color::Blue);

		sliderText.setFont(font);
		sliderText.setCharacterSize(60);
		sliderText.setScale(Vector2f(.2, .2));
		sliderText.setString("rotation 0");
		sliderText.setFillColor(Color::Black);
		sliderText.setPosition(_position + Vector2f(.1, -20));

	}

	void DrawSlider(RenderWindow& window) {
		window.draw(sliderShape);
		window.draw(sliderButton);
		window.draw(sliderText);

	}



	float computeValue(float value) {
		return value - minX + 1;
	}

	void SliderLogic(Vector2f mouseCoords, Event event) {
		if (isMouseOnButtonSlider(mouseCoords) && !isDraggingButton && event.type == Event::MouseButtonPressed)
		{
			isDraggingButton = true;
			previousMouseCoords = mouseCoords;
		}
		if (isDraggingButton)
		{
			string SliderText = "rotation: ";
			sliderText.setString(SliderText.append(to_string((int)computeValue(sliderButton.getPosition().x))));
			cout << computeValue(sliderButton.getPosition().x) << '\n';
			currentMouseCoords = mouseCoords;
			auto offset = Vector2f(currentMouseCoords.x - previousMouseCoords.x, currentMouseCoords.y - previousMouseCoords.y);
			previousMouseCoords = currentMouseCoords;
			sliderButton.setPosition(sliderButton.getPosition().x + offset.x, sliderButton.getPosition().y);
		}
		if (event.type == Event::MouseButtonReleased)
		{
			isDraggingButton = false;
		}

		if (sliderButton.getPosition().x < minX)
			sliderButton.setPosition(minX, sliderButton.getPosition().y);

		if (sliderButton.getPosition().x > maxX)
			sliderButton.setPosition(maxX, sliderButton.getPosition().y);
	}

	bool isMouseOnButtonSlider(Vector2f mouseCoords) {
		auto shapeBounds = sliderButton.getGlobalBounds();
		if (mouseCoords.x > shapeBounds.left && mouseCoords.x < shapeBounds.left + shapeBounds.width && mouseCoords.y > shapeBounds.top && mouseCoords.y < shapeBounds.top + shapeBounds.height) {
			return true;
		}
		return false;
	}
};

class Components
{
public:
	Components() = default;

	VertexArray ComputeDiodeDraw(Vector2f position, Vector2f size, float rotation) {
		//file = fopen("C:\\Users\\plays\\source\\repos\\SFML-Electron\\x64\\Debug\\Components\\Diode.txt", "r");
		ifstream fin("C:\\Users\\plays\\source\\repos\\SFML-Electron\\x64\\Debug\\Components\\Diode.txt");
		char readedLine[256] = "undefined";
		VertexArray shape(sf::Lines, 12);
		do
		{
			fin.getline(readedLine, 256);
		} while (readedLine[0] != 'F');

		for (int i = 0; i <= 5; i++)
		{
			fin.getline(readedLine, 256);
			switch (readedLine[0])
			{
			case 'L':
				int values[10] = { 0 };
				char* p = strtok(readedLine, " ");
				int j = 0;


				while (p != nullptr)
				{
					values[j++] = atoi(p);
					p = strtok(NULL, " ");
				}


				shape[i * 2].position = Vector2f(position.x + values[1], position.y + values[2]);
				shape[i * 2 + 1].position = Vector2f(position.x + values[3], position.y + values[4]);
				shape[i * 2].color = Color::Black;
				shape[i * 2 + 1].color = Color::Black;
				break;
			}

		}
		fin.close();
		return shape;
	}
	VertexArray ComputeCapacitatorDraw(Vector2f position, Vector2f size, float rotation)
	{
		ifstream fin("C:\\Users\\plays\\source\\repos\\SFML-Electron\\x64\\Debug\\Components\\Capacitator.txt");
		VertexArray shape(sf::Lines, 8);
		char readedLine[256] = "undefined";
		do
		{
			fin.getline(readedLine, 256);
		} while (readedLine[0] != 'F');

		for (int i = 0; i <= 3; i++)
		{
			fin.getline(readedLine, 256);
			switch (readedLine[0])
			{
			case 'L':
				int values[10] = { 0 };
				char* p = strtok(readedLine, " ");
				int j = 0;


				while (p != nullptr)
				{
					values[j++] = atoi(p);
					p = strtok(NULL, " ");
				}


				shape[i * 2].position = Vector2f(position.x + values[1], position.y + values[2]);
				shape[i * 2 + 1].position = Vector2f(position.x + values[3], position.y + values[4]);
				shape[i * 2].color = Color::Black;
				shape[i * 2 + 1].color = Color::Black;
				break;
			}

		}
		fin.close();
		return shape;
	}

	VertexArray ComputeFuseDraw(Vector2f position, Vector2f size, float rotation)
	{
		ifstream fin("C:\\Users\\plays\\source\\repos\\SFML-Electron\\x64\\Debug\\Components\\Fuse.txt");
		char readedLine[256] = "undefined";
		VertexArray shape(Lines, 10);
		do
		{
			fin.getline(readedLine, 256);
		} while (readedLine[0] != 'F');

		for (int i = 0; i <= 5; i++)
		{
			fin.getline(readedLine, 256);
			switch (readedLine[0])
			{
			case 'L':
				int values[10] = { 0 };
				char* p = strtok(readedLine, " ");
				int j = 0;


				while (p != nullptr)
				{
					values[j++] = atoi(p);
					p = strtok(NULL, " ");
				}


				shape[i * 2].position = Vector2f(position.x + values[1], position.y + values[2]);
				shape[i * 2 + 1].position = Vector2f(position.x + values[3], position.y + values[4]);
				shape[i * 2].color = Color::Black;
				shape[i * 2 + 1].color = Color::Black;
				break;


			}

		}
		fin.close();
		return shape;
	}

	pair<VertexArray, CircleShape>  ComputeBulbDraw(Vector2f position, Vector2f size, float rotation)
	{
		ifstream fin("C:\\Users\\plays\\source\\repos\\SFML-Electron\\x64\\Debug\\Components\\Bulb.txt");
		char readedLine[256] = "undefined";
		VertexArray shape(Lines, 8);
		CircleShape circleShape;
		do
		{
			fin.getline(readedLine, 256);
		} while (readedLine[0] != 'F');

		for (int i = 0; i <= 5; i++)
		{
			fin.getline(readedLine, 256);
			switch (readedLine[0])
			{
			case 'L':
			{
				int values[10] = { 0 };
				char* p = strtok(readedLine, " ");
				int j = 0;


				while (p != nullptr)
				{
					values[j++] = atoi(p);
					p = strtok(NULL, " ");
				}


				shape[i * 2].position = Vector2f(position.x + values[1], position.y + values[2]);
				shape[i * 2 + 1].position = Vector2f(position.x + values[3], position.y + values[4]);
				shape[i * 2].color = Color::Black;
				shape[i * 2 + 1].color = Color::Black;
			}
			break;
			case 'C':
			{
				int values[10] = { 0 };
				char* p = strtok(readedLine, " ");
				int j = 0;


				while (p != nullptr)
				{
					values[j++] = atoi(p);
					p = strtok(NULL, " ");
				}

				circleShape = CircleShape(values[1]);
				circleShape.setPosition(Vector2f(position.x + values[2], position.y + values[3]));
				circleShape.setFillColor(Color::White);
				circleShape.setOutlineColor(Color::Black);
				circleShape.setOutlineThickness(.7);

			}


			}

		}
		fin.close();
		return make_pair(shape, circleShape);
	}

	VertexArray ComputeCellDraw(Vector2f position, Vector2f size, float rotation) {
		ifstream fin("C:\\Users\\plays\\source\\repos\\SFML-Electron\\x64\\Debug\\Components\\ElectricalCell.txt");


		char readedLine[256] = "undefined";
		VertexArray shape(sf::Lines, 8);
		do
		{
			fin.getline(readedLine, 256);
		} while (readedLine[0] != 'F');

		for (int i = 0; i <= 3; i++)
		{
			fin.getline(readedLine, 256);
			switch (readedLine[0])
			{
			case 'L':
				int values[10] = { 0 };
				char* p = strtok(readedLine, " ");
				int j = 0;


				while (p != nullptr)
				{
					values[j++] = atoi(p);
					p = strtok(NULL, " ");
				}


				shape[i * 2].position = Vector2f(position.x + values[1], position.y + values[2]);
				shape[i * 2 + 1].position = Vector2f(position.x + values[3], position.y + values[4]);
				shape[i * 2].color = Color::Black;
				shape[i * 2 + 1].color = Color::Black;
				break;
			}

		}
		fin.close();
		return shape;

	}

	pair<VertexArray, CircleShape>  ComputeAmmeterDraw(Vector2f position, Vector2f size, float rotation)
	{
		ifstream fin("C:\\Users\\plays\\source\\repos\\SFML-Electron\\x64\\Debug\\Components\\Ammeter.txt");
		char readedLine[256] = "undefined";
		VertexArray shape(Lines, 10);
		CircleShape circleShape;
		do
		{
			fin.getline(readedLine, 256);
		} while (readedLine[0] != 'F');

		for (int i = 0; i <= 6; i++)
		{
			fin.getline(readedLine, 256);
			switch (readedLine[0])
			{
			case 'L':
			{
				int values[10] = { 0 };
				char* p = strtok(readedLine, " ");
				int j = 0;


				while (p != nullptr)
				{
					values[j++] = atoi(p);
					p = strtok(NULL, " ");
				}


				shape[i * 2].position = Vector2f(position.x + values[1], position.y + values[2]);
				shape[i * 2 + 1].position = Vector2f(position.x + values[3], position.y + values[4]);
				shape[i * 2].color = Color::Black;
				shape[i * 2 + 1].color = Color::Black;
			}
			break;
			case 'C':
			{
				int values[10] = { 0 };
				char* p = strtok(readedLine, " ");
				int j = 0;


				while (p != nullptr)
				{
					values[j++] = atoi(p);
					p = strtok(NULL, " ");
				}

				circleShape = CircleShape(values[1]);
				circleShape.setPosition(Vector2f(position.x + values[2], position.y + values[3]));
				circleShape.setFillColor(Color::White);
				circleShape.setOutlineColor(Color::Black);
				circleShape.setOutlineThickness(.7);

			}


			}

		}
		fin.close();
		return make_pair(shape, circleShape);
	}

	pair<VertexArray, CircleShape>  ComputeVoltmeterDraw(Vector2f position, Vector2f size, float rotation)
	{
		ifstream fin("C:\\Users\\plays\\source\\repos\\SFML-Electron\\x64\\Debug\\Components\\\Voltmeter.txt");
		char readedLine[256] = "undefined";
		VertexArray shape(Lines, 8);
		CircleShape circleShape;
		do
		{
			fin.getline(readedLine, 256);
		} while (readedLine[0] != 'F');

		for (int i = 0; i <= 5; i++)
		{
			fin.getline(readedLine, 256);
			switch (readedLine[0])
			{
			case 'L':
			{
				int values[10] = { 0 };
				char* p = strtok(readedLine, " ");
				int j = 0;


				while (p != nullptr)
				{
					values[j++] = atoi(p);
					p = strtok(NULL, " ");
				}


				shape[i * 2].position = Vector2f(position.x + values[1], position.y + values[2]);
				shape[i * 2 + 1].position = Vector2f(position.x + values[3], position.y + values[4]);
				shape[i * 2].color = Color::Black;
				shape[i * 2 + 1].color = Color::Black;
			}
			break;
			case 'C':
			{
				int values[10] = { 0 };
				char* p = strtok(readedLine, " ");
				int j = 0;


				while (p != nullptr)
				{
					values[j++] = atoi(p);
					p = strtok(NULL, " ");
				}

				circleShape = CircleShape(values[1]);
				circleShape.setPosition(Vector2f(position.x + values[2], position.y + values[3]));
				circleShape.setFillColor(Color::White);
				circleShape.setOutlineColor(Color::Black);
				circleShape.setOutlineThickness(.7);

			}


			}

		}
		fin.close();
		return make_pair(shape, circleShape);
	}

	pair<VertexArray, CircleShape>  ComputeTransistorDraw(Vector2f position, Vector2f size, float rotation)
	{
		ifstream fin("C:\\Users\\plays\\source\\repos\\SFML-Electron\\x64\\Debug\\Components\\Transistor.txt");
		char readedLine[256] = "undefined";
		VertexArray shape(Lines, 12);
		CircleShape circleShape;
		do
		{
			fin.getline(readedLine, 256);
		} while (readedLine[0] != 'F');

		for (int i = 0; i <= 8; i++)
		{
			fin.getline(readedLine, 256);
			switch (readedLine[0])
			{
			case 'L':
			{
				int values[10] = { 0 };
				char* p = strtok(readedLine, " ");
				int j = 0;


				while (p != nullptr)
				{
					values[j++] = atoi(p);
					p = strtok(NULL, " ");
				}


				shape[i * 2].position = Vector2f(position.x + values[1], position.y + values[2]);
				shape[i * 2 + 1].position = Vector2f(position.x + values[3], position.y + values[4]);
				shape[i * 2].color = Color::Black;
				shape[i * 2 + 1].color = Color::Black;
			}
			break;
			case 'C':
			{
				int values[10] = { 0 };
				char* p = strtok(readedLine, " ");
				int j = 0;


				while (p != nullptr)
				{
					values[j++] = atoi(p);
					p = strtok(NULL, " ");
				}

				circleShape = CircleShape(values[1]);
				circleShape.setPosition(Vector2f(position.x + values[2], position.y + values[3]));
				circleShape.setFillColor(Color::White);
				circleShape.setOutlineColor(Color::Black);
				circleShape.setOutlineThickness(.7);

			}


			}

		}
		fin.close();
		return make_pair(shape, circleShape);
	}

	VertexArray ComputeResistorDraw(Vector2f position, Vector2f size, float rotation)
	{
		ifstream fin("C:\\Users\\plays\\source\\repos\\SFML-Electron\\x64\\Debug\\Components\\Resistor.txt");
		char readedLine[256] = "undefined";
		VertexArray shape(Lines, 12);
		do
		{
			fin.getline(readedLine, 256);
		} while (readedLine[0] != 'F');

		for (int i = 0; i <= 6; i++)
		{
			fin.getline(readedLine, 256);
			switch (readedLine[0])
			{
			case 'L':
				int values[10] = { 0 };
				char* p = strtok(readedLine, " ");
				int j = 0;


				while (p != nullptr)
				{
					values[j++] = atoi(p);
					p = strtok(NULL, " ");
				}


				shape[i * 2].position = Vector2f(position.x + values[1], position.y + values[2]);
				shape[i * 2 + 1].position = Vector2f(position.x + values[3], position.y + values[4]);
				shape[i * 2].color = Color::Black;
				shape[i * 2 + 1].color = Color::Black;
				break;


			}

		}
		fin.close();
		return shape;
	}

	void SaveDiodeDraw(Vector2f position, Vector2f size, float angleRotation, VertexArray& shape)
	{
		shape = ComputeDiodeDraw(position, size, angleRotation);
	}

	void SaveCellDraw(Vector2f position, Vector2f size, float angleRotation, VertexArray& shape)
	{
		shape = ComputeCellDraw(position, size, angleRotation);
	}

	void SaveCapacitatorDraw(Vector2f position, Vector2f size, float angleRotation, VertexArray& shape)
	{
		shape = ComputeCapacitatorDraw(position, size, angleRotation);
	}

	void SaveFuseDraw(Vector2f position, Vector2f size, float angleRotation, VertexArray& shape)
	{
		shape = ComputeFuseDraw(position, size, angleRotation);
	}

	void SaveResistorDraw(Vector2f position, Vector2f size, float angleRotation, VertexArray& shape)
	{
		shape = ComputeResistorDraw(position, size, angleRotation);
	}

	pair<VertexArray, CircleShape> SaveBulbDraw(Vector2f position, Vector2f size, float angleRotation)
	{
		return ComputeBulbDraw(position, size, angleRotation);
	}

	pair<VertexArray, CircleShape> SaveAmmeterDraw(Vector2f position, Vector2f size, float angleRotation)
	{
		return ComputeAmmeterDraw(position, size, angleRotation);
	}

	pair<VertexArray, CircleShape> SaveVoltmeterDraw(Vector2f position, Vector2f size, float angleRotation)
	{
		return ComputeVoltmeterDraw(position, size, angleRotation);
	}

	pair<VertexArray, CircleShape> SaveTransistorDraw(Vector2f position, Vector2f size, float angleRotation)
	{
		return ComputeTransistorDraw(position, size, angleRotation);
	}
};

class ElectricalComponent : public MainClass, Components
{

public:
	VertexArray shape;
	CircleShape circleShape;
	Vector2f position;
	Vector2f size;
	enum Type {
		Diode,
		Transistor,
		Capacitator,
		Ammeter,
		Bulb,
		Cell,
		Fuse,
		Resistor,
		Voltmeter

	}type;
	Sprite componentSprite;
	float rotation = 0;

	ElectricalComponent(Type _type, Vector2f _position, Vector2f _size, float _rotation) {
		type = _type;
		position = Vector2f(_position.x - _size.x, _position.y - _size.y);
		size = _size;
		rotation = _rotation;

		//LoadSprite();
		LoadComponentShape();
	}

	void LoadComponentShape() {
		switch (type) {
		case Diode:
			SaveDiodeDraw(position, size, rotation, shape);
			break;
		case Transistor:
		{
			pair<VertexArray, CircleShape> var = ComputeTransistorDraw(position, size, rotation);
			shape = var.first;
			circleShape = var.second;
		}
		break;
		case Capacitator:
			SaveCapacitatorDraw(position, size, rotation, shape);
			break;
		case Ammeter:
		{
			pair<VertexArray, CircleShape> var = ComputeAmmeterDraw(position, size, rotation);
			shape = var.first;
			circleShape = var.second;
		}
		break;
		case Bulb:
		{
			pair<VertexArray, CircleShape> var = ComputeBulbDraw(position, size, rotation);
			shape = var.first;
			circleShape = var.second;
		}

		break;
		case Cell:
			SaveCellDraw(position, size, rotation, shape);
			break;
		case Voltmeter:
		{
			pair<VertexArray, CircleShape> var = ComputeVoltmeterDraw(position, size, rotation);
			shape = var.first;
			circleShape = var.second;
		}
		break;
		case Resistor:
			SaveResistorDraw(position, size, rotation, shape);
			break;


		case Fuse:
			SaveFuseDraw(position, size, rotation, shape);
			break;

		}
	}

	bool isMouseOnComponent(Vector2f mouseCoords)
	{
		auto shapeBounds = shape.getBounds();
		if (mouseCoords.x > shapeBounds.left && mouseCoords.x < shapeBounds.left + shapeBounds.width && mouseCoords.y > shapeBounds.top && mouseCoords.y < shapeBounds.top + shapeBounds.height) {
			return true;
		}
		return false;
	}


	Sprite DrawSprite() {
		componentSprite.setScale(size);
		componentSprite.setPosition(position);
		return componentSprite;
	}

	void setPosition(Vector2f offset) {
		for (int i = 0; i < shape.getVertexCount(); i++)
		{
			shape[i].position = Vector2f(shape[i].position.x + offset.x, shape[i].position.y + offset.y);
		}
	}
	void hideObject() {

	}



};

class Button : public MainClass
{
public:
	enum State {
		INACTIVE,
		HOVERED,
		CLICKED,
		SELECTED,
	}buttonState;

	enum Type {
		Null,
		Cell,
		Resistor,
		Bulb,
		Ammeter,
		Voltmeter,
		Capacitor,
		Transistor,
		Fuse,
		Diode,

	}typeToCast;
	RectangleShape buttonShape;
	Text buttonText;
	Vector2f position;
	Vector2f size;

	float rotation = 0;

	Button(Vector2f _position, Vector2f _size, float _rotation, Type _typeToCast, string _buttonText) {
		position = Vector2f(_position.x - _size.x / 2, _position.y - _size.y / 2);
		size = _size;
		rotation = _rotation;
		typeToCast = _typeToCast;
		buttonText.setString(_buttonText);
		
		buttonState = INACTIVE;

		DrawButton();
	}




	void DrawButton() {
		buttonShape.setSize(size);
		buttonShape.setOutlineColor(Color(100,100,100, 255));
		buttonShape.setOutlineThickness(1);
		buttonShape.setPosition(position);

		buttonText.setFont(font);
		buttonText.setCharacterSize(120);
		buttonText.setScale(Vector2f(.13, .13));
		buttonText.setFillColor(Color::White);


		//temporary solution //TODO: center the text in center of the button
		buttonText.setPosition(Vector2f(position.x + size.x / 8 + 15, position.y + size.y / 8));
	}

	void InstantiateComponent(Type type, ElectricalComponent* components[], int& electricalCount, Vector2f mouseCoords, Vector2f defaultSize) {
		switch (type) {

		case Diode:
		{

			ElectricalComponent* diode = new ElectricalComponent(ElectricalComponent::Diode, Vector2f(mouseCoords.x - size.x / 4, mouseCoords.y - size.y), Vector2f(defaultSize.x, defaultSize.y), 0);
			components[++electricalCount] = diode;
		}
		break;
		case Fuse:
		{
			ElectricalComponent* fuse = new ElectricalComponent(ElectricalComponent::Fuse, Vector2f(mouseCoords.x - size.x / 4, mouseCoords.y - size.y), Vector2f(defaultSize.x, defaultSize.y), 0);
			components[++electricalCount] = fuse;

		}
		break;
		case Voltmeter:
		{
			ElectricalComponent* voltmeter = new ElectricalComponent(ElectricalComponent::Voltmeter, Vector2f(mouseCoords.x - size.x / 4, mouseCoords.y - size.y), Vector2f(defaultSize.x, defaultSize.y), 0);
			components[++electricalCount] = voltmeter;

		}
		break;
		case Ammeter:
		{
			ElectricalComponent* ammeter = new ElectricalComponent(ElectricalComponent::Ammeter, Vector2f(mouseCoords.x - size.x / 4, mouseCoords.y - size.y), Vector2f(defaultSize.x, defaultSize.y), 0);
			components[++electricalCount] = ammeter;
		}
		break;

		case Capacitor:
		{
			ElectricalComponent* capacitator = new ElectricalComponent(ElectricalComponent::Capacitator, Vector2f(mouseCoords.x - size.x / 4, mouseCoords.y - size.y), Vector2f(defaultSize.x, defaultSize.y), 0);
			components[++electricalCount] = capacitator;
		}
		break;
		case Transistor:
		{
			ElectricalComponent* transistor = new ElectricalComponent(ElectricalComponent::Transistor, Vector2f(mouseCoords.x - size.x / 4, mouseCoords.y - size.y), Vector2f(defaultSize.x, defaultSize.y), 0);
			components[++electricalCount] = transistor;
		}
		break;
		case Bulb:
		{
			ElectricalComponent* bulb = new ElectricalComponent(ElectricalComponent::Bulb, Vector2f(mouseCoords.x - size.x / 4, mouseCoords.y - size.y), Vector2f(defaultSize.x, defaultSize.y), 0);
			components[++electricalCount] = bulb;
		}
		break;
		case Cell:
		{
			ElectricalComponent* cell = new ElectricalComponent(ElectricalComponent::Cell, Vector2f(mouseCoords.x - size.x / 4, mouseCoords.y - size.y), Vector2f(defaultSize.x, defaultSize.y), 0);
			components[++electricalCount] = cell;
		}
		break;
		case Resistor:
		{
			ElectricalComponent* resistor = new ElectricalComponent(ElectricalComponent::Resistor, Vector2f(mouseCoords.x - size.x / 4, mouseCoords.y - size.y), Vector2f(defaultSize.x, defaultSize.y), 0);
			components[++electricalCount] = resistor;
		}
		break;
		}
	}





};

