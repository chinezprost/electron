//sfml library
#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>

//Interface_Object.h class
#include "Interface_Object.h"

#define nullptr NULL
#define MAX_COMPONENTS 100
#define SCREEN_DIVIDER 4


using namespace std;
using namespace sf;

#pragma region variables
//window related variables
int windowHeight, windowWidth;
RenderWindow window;

//two-dimensional pair of float elements
Vector2f centerScreen;
Vector2f previousMouseCoords;

//self-explinatory booleans
bool isHoldingClick = false;
bool isDraggingComponent = false;

//curent state of project
enum State {
	MainMenu,
	Project,
	Info,
}states;

//static-buttons
Button* mainMenu_buttons[MAX_COMPONENTS];
Button* project_buttons[MAX_COMPONENTS];

//electrical components related
ElectricalComponent* project_components[MAX_COMPONENTS];
ElectricalComponent* draggedComponent;

//line
list<RectangleShape*> line;

//counters
int buttonCount = 0;
int projectbuttonCount = 0;
int projectComponentCount = 0;

//selected components
int selectedComponentIndex = -1;
FloatRect selectedComponentBounds;

//mouse variable
Mouse myMouse;

//line draw
Vector2f pointA, pointB;

//view-port shapes
RectangleShape use_space;

//view-port sliders
Slider rotationSlider(Vector2f(centerScreen.x + 810, centerScreen.y + 470));


//font
Font font;

bool canComponentsBeSelected = false;

#pragma endregion

bool isInDrawableArea(Vector2f mouseCoords, FloatRect shapeBounds) {
	if (mouseCoords.x > shapeBounds.left &&
		mouseCoords.x < shapeBounds.left + shapeBounds.width &&
		mouseCoords.y > shapeBounds.top &&
		mouseCoords.y < shapeBounds.top + shapeBounds.height)
		return true;

	return false;
}

void drawConnectionLines(Vector2f linepos, Vector2f linescale, float rot)
{
	RectangleShape* newLine = new RectangleShape(linepos);
	newLine->setRotation(rot);
	newLine->setScale(linescale);
	newLine->setFillColor(Color::Black);
	

	line.push_back(newLine);
	
}


void computeLines(Vector2f pointA, Vector2f pointB) {

	auto distance = abs(pointB.x - pointA.x);
	if (!(pointB.x < pointA.x && pointB.y < pointA.y)) {
		auto aux = pointA;
		pointA = pointB;
		pointB = aux;

	}
	if (pointB.x < pointA.x && pointB.y < pointA.y) {
		RectangleShape* line1 = new RectangleShape(Vector2f(distance / 2, .5));
		line1->setFillColor(Color::Black);
		RectangleShape* line2 = new RectangleShape(Vector2f(distance / 2, .5));
		line2->setFillColor(Color::Black);
		RectangleShape* line3 = new RectangleShape(Vector2f(abs(pointB.y - pointA.y), .5));
		line3->setFillColor(Color::Black);

		line1->setPosition(Vector2f(pointA.x - distance / 2, pointA.y));
		line2->setPosition(pointB);
		line3->setRotation(90);
		line3->setPosition(Vector2f(pointA.x - distance / 2, pointB.y));

		line.push_back(line1);
		line.push_back(line2);
		line.push_back(line3);

		return;
	}




	if (pointB.x < pointA.x && pointA.y < pointB.y) {
		auto aux = pointA;
		pointA = pointB;
		pointB = aux;
	}
	if (!(pointA.x < pointB.x && pointA.y < pointB.y)) {
		RectangleShape* line1 = new RectangleShape(Vector2f(distance / 2, .5));
		line1->setFillColor(Color::Black);
		RectangleShape* line2 = new RectangleShape(Vector2f(distance / 2, .5));
		line2->setFillColor(Color::Black);
		RectangleShape* line3 = new RectangleShape(Vector2f(abs(pointB.y - pointA.y), .5));
		line3->setFillColor(Color::Black);

		line1->setPosition(Vector2f(pointA));
		line2->setPosition(Vector2f(pointB.x - distance / 2, pointB.y));
		line3->setRotation(90);
		line3->setPosition(Vector2f(pointA.x + distance / 2, pointB.y));

		line.push_back(line1);
		line.push_back(line2);
		line.push_back(line3);
	}





}

void drawLines() {
	for (auto x : line) {
		window.draw(*x);
	}
}

bool anySelectedButton() {
	for (int i = 1; i <= projectbuttonCount; i++) {
		if (project_buttons[i]->buttonState == Button::SELECTED)
			return true;
	}
	return false;
}

void deleteComponent(int index)
{
	int i = index;

	project_components[i] = nullptr;
	for (i; i < projectComponentCount; i++)
		project_components[i] = project_components[i + 1];
	project_components[i] = nullptr;
	projectComponentCount--;

}

void drawLinesToBounds(FloatRect bounds, float offset = 5)
{
	bounds.left -= offset;
	bounds.height += offset * 2.125;
	bounds.top -= offset;
	bounds.width += offset * 2.125;

	VertexArray lines(Lines, 8);
	lines[0].position = Vector2f(bounds.left, bounds.top);
	lines[0].color = Color::Red;

	lines[1].position = Vector2f(bounds.left + bounds.width, bounds.top);
	lines[1].color = Color::Red;

	lines[2].position = Vector2f(bounds.left + bounds.width, bounds.top);
	lines[2].color = Color::Red;

	lines[3].position = Vector2f(bounds.left + bounds.width, bounds.top + bounds.height);
	lines[3].color = Color::Red;

	lines[4].position = Vector2f(bounds.left + bounds.width, bounds.top + bounds.height);
	lines[4].color = Color::Red;

	lines[5].position = Vector2f(bounds.left, bounds.top + bounds.height);
	lines[5].color = Color::Red;

	lines[6].position = Vector2f(bounds.left, bounds.top + bounds.height);
	lines[6].color = Color::Red;

	lines[7].position = Vector2f(bounds.left, bounds.top);
	lines[7].color = Color::Red;


	/*for (int i = 0; i < 4; i++)
	{
		cout << "X: " << lines[i].position.x << "Y: " << lines[i].position.y << '\n';
	}
	cout << '\n';
	*/


	window.draw(lines);
}

void loadFont()
{
	if (!font.loadFromFile("Arialn.ttf")) {
		cout << "Couldn't load font. You must've install the above font in the same folder as the project to continue. Returning -1.";
		exit(0);
	}
}

bool areCoordsInBounds(Vector2f coords, FloatRect bounds)
{
	if (coords.x > bounds.left && coords.x < bounds.left + bounds.width && coords.y > bounds.top && coords.y < bounds.top + bounds.height)
		return true;
	return false;
}

void saveToFile()
{
	auto fout = ofstream("C:\\Users\\plays\\source\\repos\\SFML-Electron\\x64\\Debug\\Save\\output.txt");
	fout << projectComponentCount << '\n';
	for (int i = 1; i <= projectComponentCount; i++)
	{
		fout << project_components[i]->type << " ";
		fout << project_components[i]->position.x << " " << project_components[i]->position.y << " ";
		fout << project_components[i]->rotation << " ";
		fout << '\n';
	}
	fout << '\n';
	fout << line.size() << '\n';
	for (list<RectangleShape*>::iterator element = line.begin(); element != line.end(); element++)
	{
		fout << (*element)->getPosition().x << " ";
		fout << (*element)->getPosition().y << " ";

		fout << (*element)->getScale().x << " ";
		fout << (*element)->getScale().y << " ";

		fout << (*element)->getRotation() << '\n';
	}

	fout.close();
}

void resetProject()
{
	memset(project_components, 0, sizeof(project_components));
	projectComponentCount = 0;
	line.clear();
	selectedComponentIndex = -1;
}

Button::Type returnType(int input)
{
	switch (input)
	{
	case 0:
		return Button::Null;
	case 1:
		return Button::Cell;
	case 2:
		return Button::Resistor;
	case 3:
		return Button::Ammeter;
	case 4:
		return Button::Voltmeter;
	case 5:
		return Button::Capacitor;
	case 6:
		return Button::Transistor;
	case 7:
		return Button::Fuse;
	case 8:
		return Button::Diode;
	default:
		cout << "an error happened, couldn't load file";
		return Button::Null;
	}

}

void loadFromFile()
{

	auto fin = ifstream("C:\\Users\\plays\\source\\repos\\SFML-Electron\\x64\\Debug\\Save\\output.txt");
	resetProject();
	int count;
	fin >> count;
	int type; Vector2f position; float rotation;
	projectComponentCount = 0;
	for (int i = 1; i <= count; i++)
	{
		fin >> type; fin >> position.x; fin >> position.y; fin >> rotation;
		project_buttons[1]->InstantiateComponent(returnType(type), project_components, projectComponentCount, position, Vector2f(1, 1));
	}

	int lineCount;
	fin >> lineCount;

	line.clear();
	Vector2f linePos; Vector2f lineScale; float rot;
	for (int i = 1; i <= lineCount; i++)
	{
		fin >> linePos.x >> linePos.y >> lineScale.x >> lineScale.y >> rot;
		drawConnectionLines(linePos, lineScale, rot);
		
	}

	fin.close();




}






void init()
{
	loadFont();
}

int main() {
	//initialize loaded-objects (ex: font)
	init();

	windowHeight = VideoMode::getDesktopMode().height;
	windowWidth = VideoMode::getDesktopMode().width;
	window.create(VideoMode(windowWidth / SCREEN_DIVIDER * 2, windowHeight / SCREEN_DIVIDER * 2), "SFML Electron");

	states = MainMenu;

	window.setVerticalSyncEnabled(true);
	centerScreen.x = window.getPosition().x;
	centerScreen.y = window.getPosition().y;

	//declaration of static elements within the project
	Vector2f defaultComponentButtonSize = Vector2f(100, 30);
	Vector2f defaultProjectButtonSize = Vector2f(155, 30);
#pragma region static-declarated buttons
	Button button_mainMenu_newProject(Vector2f(centerScreen.x, centerScreen.y - 100), Vector2f(150, 63), 0, Button::Null, "New project");
	mainMenu_buttons[++buttonCount] = &button_mainMenu_newProject;
	Button button_mainMenu_openProject(Vector2f(centerScreen.x, centerScreen.y - 20), Vector2f(150, 63), 0, Button::Null, "Open Project");
	mainMenu_buttons[++buttonCount] = &button_mainMenu_openProject;
	Button button_mainMenu_info(Vector2f(centerScreen.x, centerScreen.y + 60), Vector2f(150, 63), 0, Button::Null, "Info");
	mainMenu_buttons[++buttonCount] = &button_mainMenu_info;
	Button button_mainMenu_exit(Vector2f(centerScreen.x, centerScreen.y + 140), Vector2f(150, 63), 0, Button::Null, "Exit");
	mainMenu_buttons[++buttonCount] = &button_mainMenu_exit;
	Button button_project_cell(Vector2f(centerScreen.x - 425, centerScreen.y - 200), defaultComponentButtonSize, 0, Button::Cell, "Cell");
	project_buttons[++projectbuttonCount] = &button_project_cell;
	Button button_project_resistor(Vector2f(centerScreen.x - 425, centerScreen.y - 150), defaultComponentButtonSize, 0, Button::Resistor, "Resistor");
	project_buttons[++projectbuttonCount] = &button_project_resistor;
	Button button_project_bulb(Vector2f(centerScreen.x - 425, centerScreen.y - 100), defaultComponentButtonSize, 0, Button::Bulb, "Bulb");
	project_buttons[++projectbuttonCount] = &button_project_bulb;
	Button button_project_diode(Vector2f(centerScreen.x - 425, centerScreen.y - 50), defaultComponentButtonSize, 0, Button::Diode, "Diode");
	project_buttons[++projectbuttonCount] = &button_project_diode;
	Button button_project_ammeter(Vector2f(centerScreen.x - 425, centerScreen.y), defaultComponentButtonSize, 0, Button::Ammeter, "Ammeter");
	project_buttons[++projectbuttonCount] = &button_project_ammeter;
	Button button_project_voltmeter(Vector2f(centerScreen.x - 425, centerScreen.y + 50), defaultComponentButtonSize, 0, Button::Voltmeter, "Voltmeter");
	project_buttons[++projectbuttonCount] = &button_project_voltmeter;
	Button button_project_capacitor(Vector2f(centerScreen.x - 425, centerScreen.y + 100), defaultComponentButtonSize, 0, Button::Capacitor, "Capacitor");
	project_buttons[++projectbuttonCount] = &button_project_capacitor;
	Button button_project_transistor(Vector2f(centerScreen.x - 425, centerScreen.y + 150), defaultComponentButtonSize, 0, Button::Transistor, "Transistor");
	project_buttons[++projectbuttonCount] = &button_project_transistor;
	Button button_project_fuse(Vector2f(centerScreen.x - 425, centerScreen.y + 200), defaultComponentButtonSize, 0, Button::Fuse, "Fuse");
	project_buttons[++projectbuttonCount] = &button_project_fuse;
	Button button_project_back(Vector2f(centerScreen.x - 400, centerScreen.y - 250), defaultProjectButtonSize, 0, Button::Null, "Back");
	project_buttons[++projectbuttonCount] = &button_project_back;
	Button button_project_save(Vector2f(centerScreen.x - 240, centerScreen.y - 250), defaultProjectButtonSize, 0, Button::Null, "Save");
	project_buttons[++projectbuttonCount] = &button_project_save;
	Button button_project_saveas(Vector2f(centerScreen.x - 80, centerScreen.y - 250), defaultProjectButtonSize, 0, Button::Null, "Save As");
	project_buttons[++projectbuttonCount] = &button_project_saveas;
	Button button_project_open(Vector2f(centerScreen.x + 80, centerScreen.y - 250), defaultProjectButtonSize, 0, Button::Null, "Open");
	project_buttons[++projectbuttonCount] = &button_project_open;
	Button button_project_reset(Vector2f(centerScreen.x + 240, centerScreen.y - 250), defaultProjectButtonSize, 0, Button::Null, "Reset");
	project_buttons[++projectbuttonCount] = &button_project_reset;
	Button button_project_help(Vector2f(centerScreen.x + 400, centerScreen.y - 250), defaultProjectButtonSize, 0, Button::Null, "Help");
	project_buttons[++projectbuttonCount] = &button_project_help;
	Button button_project_undo(Vector2f(centerScreen.x - 415, centerScreen.y + 250), Vector2f(125, 30), 0, Button::Null, "Undo");
	project_buttons[++projectbuttonCount] = &button_project_undo;
	Button button_project_redo(Vector2f(centerScreen.x - 280, centerScreen.y + 250), Vector2f(125, 30), 0, Button::Null, "Redo");
	project_buttons[++projectbuttonCount] = &button_project_redo;
	Button button_project_edit(Vector2f(centerScreen.x - 145, centerScreen.y + 250), Vector2f(125, 30), 0, Button::Null, "Edit");
	project_buttons[++projectbuttonCount] = &button_project_edit;

	//edit button ON and OFF text
	Text edit_button_text = Text("undefined", font, 100);
	edit_button_text.setPosition(centerScreen.x - 113, centerScreen.y + 233);
	edit_button_text.setScale(.13, .13);




	Button button_project_delete(Vector2f(centerScreen.x - 10, centerScreen.y + 250), Vector2f(125, 30), 0, Button::Null, "Delete");
	project_buttons[++projectbuttonCount] = &button_project_delete;

#pragma endregion

#pragma region view-port elements init
	use_space.setSize(Vector2f(820, 450));
	use_space.setFillColor(Color(255, 255, 255, 255));
	use_space.setOutlineColor(Color::Red);
	use_space.setOutlineThickness(1);
	use_space.setPosition(Vector2f(centerScreen.x - 360, centerScreen.y - 225));
#pragma endregion

	while (window.isOpen()) {
		centerScreen.x = window.getPosition().x;
		centerScreen.y = window.getPosition().y;


		//if componenets can be selected it'll be ON and green, otherwise OFF and red
		if (canComponentsBeSelected)
		{
			edit_button_text.setString("ON");
			edit_button_text.setFillColor(Color::Green);
		}
		else
		{
			edit_button_text.setString("OFF");
			edit_button_text.setFillColor(Color::Red);
		}

		Event event;
		while (window.pollEvent(event)) {
			window.clear(Color::Black);

			if (event.type == Event::Closed) {
				window.close();
			}

			auto mouseCoords = window.mapPixelToCoords(myMouse.getPosition(window));
			switch (states) {

			case MainMenu:
				for (int i = 1; i <= buttonCount; i++) {
					auto buttonBounds = mainMenu_buttons[i]->buttonShape.getGlobalBounds();
					if (areCoordsInBounds(mouseCoords, buttonBounds))
						mainMenu_buttons[i]->buttonState = (event.type == Event::MouseButtonPressed) ? Button::CLICKED : Button::HOVERED;
					else
						mainMenu_buttons[i]->buttonState = Button::INACTIVE;
				}
				break;

			case Project:
				for (int i = 1; i <= projectbuttonCount; i++) {
					auto buttonBounds = project_buttons[i]->buttonShape.getGlobalBounds();
					if (!(areCoordsInBounds(mouseCoords, buttonBounds)) && project_buttons[i]->buttonState == Button::SELECTED && event.type == Event::MouseButtonPressed) {
						if (isInDrawableArea(mouseCoords, use_space.getGlobalBounds()))
							project_buttons[i]->InstantiateComponent(project_buttons[i]->typeToCast, project_components, projectComponentCount, mouseCoords + Vector2f(-5, 5), Vector2f(.2, .2));
						project_buttons[i]->buttonState = Button::INACTIVE;

					}
					if (areCoordsInBounds(mouseCoords, buttonBounds)) {
						if (project_buttons[i]->buttonState != Button::SELECTED)
							project_buttons[i]->buttonState = (event.type == Event::MouseButtonPressed) ? Button::CLICKED : Button::HOVERED;
					}
					else
						if (project_buttons[i]->buttonState != Button::SELECTED)
							project_buttons[i]->buttonState = Button::INACTIVE;

				}
				if (!anySelectedButton()) {
					if (isInDrawableArea(mouseCoords, use_space.getGlobalBounds()) && event.type == Event::MouseButtonPressed && !isHoldingClick) {
						isHoldingClick = true;
						pointA = Vector2f(mouseCoords.x, mouseCoords.y);
					}
					else if (isInDrawableArea(mouseCoords, use_space.getGlobalBounds()) && event.type == Event::MouseButtonReleased && isHoldingClick) {
						//if (sqrt(mouseCoords.x - pointA.x) * (mouseCoords.x - pointA.x) + (mouseCoords.y - pointA.y) * (mouseCoords.y - pointA.y) > 5)
						isHoldingClick = false;
						if (!isDraggingComponent)
							computeLines(pointA, mouseCoords);
					}
				}


				break;
			}
		}

		//iterate all buttons and check for state changes
		switch (states) {
		case MainMenu:
			for (int i = 1; i <= buttonCount; i++) {
				switch (mainMenu_buttons[i]->buttonState) {
				case(Button::INACTIVE):
					mainMenu_buttons[i]->buttonShape.setFillColor(Color::Black);
					break;
				case(Button::HOVERED):
					mainMenu_buttons[i]->buttonShape.setFillColor(Color::Cyan);
					break;
				case(Button::CLICKED):
					cout << i << " has been clicked." << endl;
					if (i == 1)
						states = Project;

					if (i == 4)
						window.close();

					mainMenu_buttons[i]->buttonState = Button::INACTIVE;
					break;
				}

			}
			break;
		case Project:
			int ok = 1;
			for (int i = 1; i <= projectbuttonCount; i++) {
				if (ok == 0)
					break;
				switch (project_buttons[i]->buttonState) {
				case(Button::INACTIVE):
					project_buttons[i]->buttonShape.setFillColor(Color::Black);
					break;
				case(Button::HOVERED):
					project_buttons[i]->buttonShape.setFillColor(Color(100, 100, 100, 255));
					break;
				case(Button::CLICKED):
					if (i >= 1 && i <= 9) {
						project_buttons[i]->buttonState = Button::SELECTED;
						continue;
					}

					cout << i << " has been clicked." << endl;
					if (i == 10)
					{
						states = MainMenu;
						selectedComponentIndex = -1;
					}
					if (i == 11)
					{
						saveToFile();

					}
					if (i == 13)
					{
						loadFromFile();
					}

					if (i == 14)
					{
						resetProject();
						ok = 0;
					}
					if (i == 18)
					{
						canComponentsBeSelected = !canComponentsBeSelected;
						selectedComponentIndex = -1;
					}

					if (i == 19)
					{
						cout << selectedComponentIndex << '\n';
						deleteComponent(selectedComponentIndex);
						selectedComponentIndex = -1;
						selectedComponentBounds = FloatRect(0, 0, 0, 0);
					}
					project_buttons[i]->buttonState = Button::INACTIVE;
					break;
				case(Button::SELECTED):
					project_buttons[i]->buttonShape.setFillColor(Color::Magenta);
					break;
				}

			}

			for (int i = 1; i <= projectComponentCount; i++) {

				if (!canComponentsBeSelected)
					break;

				//drag-and-drop logic
				if (project_components[i]->isMouseOnComponent(window.mapPixelToCoords(myMouse.getPosition(window))) && event.type == Event::MouseButtonPressed && !isDraggingComponent) {
					isDraggingComponent = true;
					previousMouseCoords = window.mapPixelToCoords(myMouse.getPosition(window));
					draggedComponent = project_components[i];
				}
				if (isDraggingComponent) {
					auto currentMouseCoords = window.mapPixelToCoords(myMouse.getPosition(window));
					auto offset = Vector2f(currentMouseCoords.x - previousMouseCoords.x, currentMouseCoords.y - previousMouseCoords.y);
					selectedComponentBounds = draggedComponent->shape.getBounds();
					previousMouseCoords = currentMouseCoords;
					draggedComponent->circleShape.setPosition(draggedComponent->circleShape.getPosition().x + offset.x, draggedComponent->circleShape.getPosition().y + offset.y);
					draggedComponent->setPosition(offset);

				}
				if (event.type == Event::MouseButtonReleased && isDraggingComponent)
				{
					isDraggingComponent = false;
					draggedComponent = nullptr;
				}




				/*else if (isDraggingComponent && event.type == Event::MouseButtonReleased)
				{
					isDraggingComponent = false;
					cout << "dragging ended" << '\n';
					auto currentMouseCoords = window.mapPixelToCoords(myMouse.getPosition(window));
					auto offset = Vector2f(currentMouseCoords.x - previousMouseCoords.x, currentMouseCoords.y - previousMouseCoords.y);
					draggedComponent->setPosition(offset);
				}*/

			}

			for (int i = 1; i <= projectComponentCount; i++)
			{
				if (!canComponentsBeSelected)
					break;
				if (event.type == Event::MouseButtonPressed && project_components[i]->isMouseOnComponent(window.mapPixelToCoords(myMouse.getPosition(window)))) {
					selectedComponentIndex = i;
					selectedComponentBounds = project_components[i]->shape.getBounds();
					break;
				}
				else if (event.type == Event::MouseButtonPressed)
				{
					selectedComponentIndex = -1;
					selectedComponentBounds = FloatRect(0, 0, 0, 0);
				}
			}
			break;
		}

		//TODO: create a list with all elements to be drawn --- for now you must manually draw each element // window.draw(class.shape)
		// 
		//draw all buttons
		if (states == MainMenu)
		{
			window.clear(Color::Black);
			for (int i = 1; i <= buttonCount; i++) {
				window.draw(mainMenu_buttons[i]->buttonShape);
				window.draw(mainMenu_buttons[i]->buttonText);

			}
		}

		if (states == Project)
		{
			window.clear(Color::Black);
			window.draw(use_space);
			for (int i = 1; i <= projectbuttonCount; i++)
			{
				window.draw(project_buttons[i]->buttonShape);
				window.draw(project_buttons[i]->buttonText);
			}

			for (int i = 1; i <= projectComponentCount; i++)
			{
				if ((project_components[i]->type == ElectricalComponent::Bulb) || (project_components[i]->type == ElectricalComponent::Ammeter) || (project_components[i]->type == ElectricalComponent::Voltmeter) || (project_components[i]->type == ElectricalComponent::Transistor))
				{
					window.draw(project_components[i]->circleShape);
					window.draw(project_components[i]->shape);
				}
				else
				{
					window.draw(project_components[i]->shape);
				}


			}

			window.draw(edit_button_text);
			rotationSlider.SliderLogic(window.mapPixelToCoords(myMouse.getPosition(window)), event);
			rotationSlider.DrawSlider(window);
			drawLines();
			//			cout << selectedComponentIndex << '\n';



		}
		//TESTING


		//computeLines(Vector2f(200, 200), Vector2f(300, 300));


		//make buttons for PROJECT buttons, check if the drawable page is in main menu or in project


		//cout << selectedComponentIndex << '\n';
		if (selectedComponentIndex != -1)
			drawLinesToBounds(selectedComponentBounds);
		window.display();
	}

}
