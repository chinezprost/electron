#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <list>
#include "Interface_Object.h"

#define null NULL
#define MAX_COMPONENTS 100
#define SCREEN_DIVIDER 4


using namespace std;
using namespace sf;




//global variables
int windowHeight, windowWidth;
struct {
	int x, y;
}centerScreen;


enum State {
	MainMenu,
	Project,
	Info,
};
Font font;
bool isHoldingClick = false;
bool isDraggingComponent = false;
ElectricalComponent* draggedComponent;
Vector2f previousMouseCoords;
Button* mainMenu_buttons[MAX_COMPONENTS];
Button* project_buttons[MAX_COMPONENTS];
window w;
ElectricalComponent* project_components[MAX_COMPONENTS];

State states;
int buttonCount = 0;
int projectbuttonCount = 0;
int projectComponentCount = 0;

bool isInDrawableArea(Vector2f mouseCoords, FloatRect shapeBounds) {
	if (mouseCoords.x > shapeBounds.left &&
		mouseCoords.x < shapeBounds.left + shapeBounds.width &&
		mouseCoords.y > shapeBounds.top &&
		mouseCoords.y < shapeBounds.top + shapeBounds.height)
		return true;

	return false;
}
list<RectangleShape*> line;
//DE FACUT SMEKER
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

void DrawLines() {
	for (auto x : line) {
		w.window.draw(*x);
	}
}


bool anySelectedButton() {
	for (int i = 1; i <= projectbuttonCount; i++) {
		if (project_buttons[i]->buttonState == Button::SELECTED)
			return true;
	}
	return false;
}

void DeleteComponent()
{
	int i;
	if (projectComponentCount > 2)
	{
		project_components[2] = nullptr;
		for (i = 2; i < projectComponentCount; i++)
			project_components[i] = project_components[i + 1];
		project_components[i] = nullptr;
		projectComponentCount--;
	}
}

void DrawLinesToBounds(FloatRect bounds)
{
	VertexArray lines(Lines, 4);
	lines[0].position = Vector2f(bounds.left, bounds.top);
	lines[0].color = Color::Red;

	lines[1].position = Vector2f(bounds.left + bounds.width, bounds.top);
	lines[1].color = Color::Red;

	lines[2].position = Vector2f(bounds.left, bounds.top - bounds.height);
	lines[2].color = Color::Red;

	lines[3].position = Vector2f(bounds.left + bounds.width, bounds.top);
	lines[3].color = Color::Red;


	w.window.draw(lines);
}

int selectedComponentIndex = -1;
FloatRect selectedComponentBounds;






int main() {
	//MainClass();
	windowHeight = VideoMode::getDesktopMode().height;
	windowWidth = VideoMode::getDesktopMode().width;
	w.window.create(VideoMode(windowWidth / SCREEN_DIVIDER * 2, windowHeight / SCREEN_DIVIDER * 2), "SFML Electron");
	states = MainMenu;
	if (!font.loadFromFile("Arialn.ttf")) {
		cout << "Couldn't load font. You must've install the above font in the same folder as the project to continue. Returning -1.";
		return -1;
	}
	Mouse myMouse;



	w.window.setVerticalSyncEnabled(true);
	centerScreen.x = w.window.getPosition().x;
	centerScreen.y = w.window.getPosition().y;

	//declaration of static elements within the project

	Vector2f pointA, pointB;
	Button button_mainMenu_newProject(Vector2f(centerScreen.x, centerScreen.y - 100), Vector2f(150, 63), 0, Button::Null, "New project");
	mainMenu_buttons[++buttonCount] = &button_mainMenu_newProject;

	Button button_mainMenu_openProject(Vector2f(centerScreen.x, centerScreen.y - 20), Vector2f(150, 63), 0, Button::Null, "Open Project");
	mainMenu_buttons[++buttonCount] = &button_mainMenu_openProject;

	Button button_mainMenu_info(Vector2f(centerScreen.x, centerScreen.y + 60), Vector2f(150, 63), 0, Button::Null, "Info");
	mainMenu_buttons[++buttonCount] = &button_mainMenu_info;

	Button button_mainMenu_exit(Vector2f(centerScreen.x, centerScreen.y + 140), Vector2f(150, 63), 0, Button::Null, "Exit");
	mainMenu_buttons[++buttonCount] = &button_mainMenu_exit;

	Button button_project_cell(Vector2f(centerScreen.x - 425, centerScreen.y - 200), Vector2f(100, 30), 0, Button::Cell, "Cell");
	project_buttons[++projectbuttonCount] = &button_project_cell;
	Button button_project_resistor(Vector2f(centerScreen.x - 425, centerScreen.y - 150), Vector2f(100, 30), 0, Button::Resistor, "Resistor");
	project_buttons[++projectbuttonCount] = &button_project_resistor;
	Button button_project_bulb(Vector2f(centerScreen.x - 425, centerScreen.y - 100), Vector2f(100, 30), 0, Button::Bulb, "Bulb");
	project_buttons[++projectbuttonCount] = &button_project_bulb;
	Button button_project_diode(Vector2f(centerScreen.x - 425, centerScreen.y - 50), Vector2f(100, 30), 0, Button::Diode, "Diode");
	project_buttons[++projectbuttonCount] = &button_project_diode;
	Button button_project_ammeter(Vector2f(centerScreen.x - 425, centerScreen.y), Vector2f(100, 30), 0, Button::Ammeter, "Ammeter");
	project_buttons[++projectbuttonCount] = &button_project_ammeter;
	Button button_project_voltmeter(Vector2f(centerScreen.x - 425, centerScreen.y + 50), Vector2f(100, 30), 0, Button::Voltmeter, "Voltmeter");
	project_buttons[++projectbuttonCount] = &button_project_voltmeter;
	Button button_project_capacitor(Vector2f(centerScreen.x - 425, centerScreen.y + 100), Vector2f(100, 30), 0, Button::Capacitor, "Capacitor");
	project_buttons[++projectbuttonCount] = &button_project_capacitor;
	Button button_project_transistor(Vector2f(centerScreen.x - 425, centerScreen.y + 150), Vector2f(100, 30), 0, Button::Transistor, "Transistor");
	project_buttons[++projectbuttonCount] = &button_project_transistor;
	Button button_project_fuse(Vector2f(centerScreen.x - 425, centerScreen.y + 200), Vector2f(100, 30), 0, Button::Fuse, "Fuse");
	project_buttons[++projectbuttonCount] = &button_project_fuse;


	Button button_project_back(Vector2f(centerScreen.x - 400, centerScreen.y - 250), Vector2f(150, 30), 0, Button::Null, "Back");
	project_buttons[++projectbuttonCount] = &button_project_back;
	Button button_project_save(Vector2f(centerScreen.x - 240, centerScreen.y - 250), Vector2f(150, 30), 0, Button::Null, "Save");
	project_buttons[++projectbuttonCount] = &button_project_save;
	Button button_project_saveas(Vector2f(centerScreen.x - 80, centerScreen.y - 250), Vector2f(150, 30), 0, Button::Null, "Save As");
	project_buttons[++projectbuttonCount] = &button_project_saveas;
	Button button_project_open(Vector2f(centerScreen.x + 80, centerScreen.y - 250), Vector2f(150, 30), 0, Button::Null, "Open");
	project_buttons[++projectbuttonCount] = &button_project_open;
	Button button_project_reset(Vector2f(centerScreen.x + 240, centerScreen.y - 250), Vector2f(150, 30), 0, Button::Null, "Reset");
	project_buttons[++projectbuttonCount] = &button_project_reset;
	Button button_project_help(Vector2f(centerScreen.x + 400, centerScreen.y - 250), Vector2f(150, 30), 0, Button::Null, "Help");
	project_buttons[++projectbuttonCount] = &button_project_help;


	Button button_project_undo(Vector2f(centerScreen.x - 415, centerScreen.y + 250), Vector2f(125, 30), 0, Button::Null, "Undo");
	project_buttons[++projectbuttonCount] = &button_project_undo;
	Button button_project_redo(Vector2f(centerScreen.x - 280, centerScreen.y + 250), Vector2f(125, 30), 0, Button::Null, "Redo");
	project_buttons[++projectbuttonCount] = &button_project_redo;
	Button button_project_edit(Vector2f(centerScreen.x - 145, centerScreen.y + 250), Vector2f(125, 30), 0, Button::Null, "Edit");
	project_buttons[++projectbuttonCount] = &button_project_edit;
	Button button_project_delete(Vector2f(centerScreen.x - 10, centerScreen.y + 250), Vector2f(125, 30), 0, Button::Null, "Delete");
	project_buttons[++projectbuttonCount] = &button_project_delete;
	RectangleShape use_space;

	//slider
	Slider rotationSlider(Vector2f(centerScreen.x + 325, centerScreen.y + 200));

	use_space.setSize(Vector2f(820, 450));
	use_space.setFillColor(Color(255, 255, 255, 255));
	use_space.setOutlineColor(Color::Red);
	use_space.setOutlineThickness(1);
	use_space.setPosition(Vector2f(centerScreen.x - 360, centerScreen.y - 225));




	while (w.window.isOpen()) {
		centerScreen.x = w.window.getPosition().x;
		centerScreen.y = w.window.getPosition().y;

		Event event;
		while (w.window.pollEvent(event)) {
			w.window.clear(Color::Black);

			if (event.type == Event::Closed) {
				w.window.close();
			}
			//buna siua 

			auto mouseCoords = w.window.mapPixelToCoords(myMouse.getPosition(w.window));
			//iterate all buttons and check if i clicked it or hovered
			switch (states) {
			case MainMenu:
				for (int i = 1; i <= buttonCount; i++) {
					auto buttonCoords = mainMenu_buttons[i]->buttonShape.getGlobalBounds();
					if (mouseCoords.x > buttonCoords.left &&
						mouseCoords.x < buttonCoords.left + buttonCoords.width &&
						mouseCoords.y > buttonCoords.top &&
						mouseCoords.y < buttonCoords.top + buttonCoords.height)
						mainMenu_buttons[i]->buttonState = (event.type == Event::MouseButtonPressed) ? Button::CLICKED : Button::HOVERED;
					else {
						mainMenu_buttons[i]->buttonState = Button::INACTIVE;
					}
				}
				break;
			case Project:



				for (int i = 1; i <= projectbuttonCount; i++) {
					auto buttonCoords = project_buttons[i]->buttonShape.getGlobalBounds();
					if (!(mouseCoords.x > buttonCoords.left &&
						mouseCoords.x < buttonCoords.left + buttonCoords.width &&
						mouseCoords.y > buttonCoords.top &&
						mouseCoords.y < buttonCoords.top + buttonCoords.height) && project_buttons[i]->buttonState == Button::SELECTED &&
						event.type == Event::MouseButtonPressed) {


						if (isInDrawableArea(mouseCoords, use_space.getGlobalBounds()))
							project_buttons[i]->InstantiateComponent(project_buttons[i]->typeToCast, project_components, projectComponentCount, mouseCoords, Vector2f(.2, .2));




						project_buttons[i]->buttonState = Button::INACTIVE;

					}


					if (mouseCoords.x > buttonCoords.left &&
						mouseCoords.x < buttonCoords.left + buttonCoords.width &&
						mouseCoords.y > buttonCoords.top &&
						mouseCoords.y < buttonCoords.top + buttonCoords.height) {
						if (project_buttons[i]->buttonState != Button::SELECTED)
							project_buttons[i]->buttonState = (event.type == Event::MouseButtonPressed) ? Button::CLICKED : Button::HOVERED;
					}
					else {
						if (project_buttons[i]->buttonState != Button::SELECTED)
							project_buttons[i]->buttonState = Button::INACTIVE;
					}
				}
				
				if (!anySelectedButton()) {
					if (isInDrawableArea(mouseCoords, use_space.getGlobalBounds()) && event.type == Event::MouseButtonPressed && !isHoldingClick) {
						isHoldingClick = true;
						pointA = Vector2f(mouseCoords.x, mouseCoords.y);
					}
					else if (isInDrawableArea(mouseCoords, use_space.getGlobalBounds()) && event.type == Event::MouseButtonReleased &&isHoldingClick) {
						//if (sqrt(mouseCoords.x - pointA.x) * (mouseCoords.x - pointA.x) + (mouseCoords.y - pointA.y) * (mouseCoords.y - pointA.y) > 5)
						isHoldingClick = false;
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
						cout << mainMenu_buttons[i]->typeToCast << " has been clicked." << endl;
						if (i == 1)
							states = Project;
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
						project_buttons[i]->buttonShape.setFillColor(Color::Red);
						break;
					case(Button::CLICKED):
						project_buttons[i]->buttonState = Button::SELECTED;
						cout << project_buttons[i]->typeToCast << " has been clicked." << endl;
						if (i == 10)
							states = MainMenu;
						if (i == 14) {
							memset(project_components, 0, sizeof(project_components));
							projectComponentCount = 0;
							line.clear();
							ok = 0;
						}
						if (i == 19)
						{
							DeleteComponent();
						}
						//project_buttons[i]->buttonState = Button::INACTIVE;
						break;
					case(Button::SELECTED):
						project_buttons[i]->buttonShape.setFillColor(Color::Magenta);
						break;
					}

				}
				
				for (int i = 1; i <= projectComponentCount; i++) {
					
					if (project_components[i]->isMouseOnComponent(w.window.mapPixelToCoords(myMouse.getPosition(w.window))) && event.type == Event::MouseButtonPressed && !isDraggingComponent) {
						isDraggingComponent = true;
						selectedComponentIndex = i;
						selectedComponentBounds = project_components[i]->shape.getBounds();
						cout << "began dragging" << '\n';
						previousMouseCoords = w.window.mapPixelToCoords(myMouse.getPosition(w.window));
						draggedComponent = project_components[i];
					}
					
					if (isDraggingComponent) {
						auto currentMouseCoords = w.window.mapPixelToCoords(myMouse.getPosition(w.window));
						auto offset = Vector2f(currentMouseCoords.x - previousMouseCoords.x, currentMouseCoords.y - previousMouseCoords.y);
						previousMouseCoords = currentMouseCoords;
						draggedComponent->circleShape.setPosition(draggedComponent->circleShape.getPosition().x + offset.x, draggedComponent->circleShape.getPosition().y + offset.y);
						draggedComponent->setPosition(offset);
					}

					if (event.type == Event::MouseButtonReleased)
					{
						isDraggingComponent = false;
						draggedComponent = nullptr;
					}
					/*else if (isDraggingComponent && event.type == Event::MouseButtonReleased)
					{
						isDraggingComponent = false;
						cout << "dragging ended" << '\n';
 						auto currentMouseCoords = w.window.mapPixelToCoords(myMouse.getPosition(w.window));
						auto offset = Vector2f(currentMouseCoords.x - previousMouseCoords.x, currentMouseCoords.y - previousMouseCoords.y);
						draggedComponent->setPosition(offset);
					}*/
					
				}
				break;
			}

			//TODO: create a list with all elements to be drawn --- for now you must manually draw each element // window.draw(class.shape)
			// 
			//draw all buttons
			if (states == MainMenu)
			{
				w.window.clear(Color::Black);
				for (int i = 1; i <= buttonCount; i++) {
					w.window.draw(mainMenu_buttons[i]->buttonShape);
					w.window.draw(mainMenu_buttons[i]->buttonText);

				}
			}

			if (states == Project)
			{
				w.window.clear(Color::Black);
				w.window.draw(use_space);
				for (int i = 1; i <= projectbuttonCount; i++)
				{
					w.window.draw(project_buttons[i]->buttonShape);
					w.window.draw(project_buttons[i]->buttonText);
				}

				for (int i = 1; i <= projectComponentCount; i++)
				{
					if ((project_components[i]->type == ElectricalComponent::Bulb) || (project_components[i]->type == ElectricalComponent::Ammeter) || (project_components[i]->type == ElectricalComponent::Voltmeter) || (project_components[i]->type == ElectricalComponent::Transistor))
					{
						w.window.draw(project_components[i]->circleShape);
						w.window.draw(project_components[i]->shape);
					}
					else
					{
						w.window.draw(project_components[i]->shape);
					}


				}

				DrawLines();
				cout << selectedComponentIndex << '\n';
				


			}
			//TESTING

		
			//computeLines(Vector2f(200, 200), Vector2f(300, 300));
				

			//make buttons for PROJECT buttons, check if the drawable page is in main menu or in project
			rotationSlider.SliderLogic(w.window.mapPixelToCoords(myMouse.getPosition(w.window)), event);
			rotationSlider.DrawSlider(w.window);

			if(selectedComponentIndex != -1)
				DrawLinesToBounds(selectedComponentBounds);
			w.window.display();
		}

	}
