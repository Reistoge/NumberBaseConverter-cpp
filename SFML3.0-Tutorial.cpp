#include "BaseConversion.cpp"
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <windows.h> // Required for WinMain
tgui::EditBox::Ptr valorTransformarPtr;
tgui::EditBox::Ptr baseInputPtr;
tgui::EditBox::Ptr valorBinarioPtr;
tgui::EditBox::Ptr valorOctalPtr;
tgui::EditBox::Ptr valorDecimalPtr;
tgui::EditBox::Ptr valorHexadecimalPtr;
tgui::EditBox::Ptr valorBaseInputPtr;
tgui::EditBox::Ptr transformBaseInputBoxPtr;
tgui::EditBox::Ptr transformBaseInputPrecisionPtr;
// 10101010101010101010101010101010101000000000
tgui::EditBox::Ptr addBaseToLayout(tgui::VerticalLayout::Ptr layout, std::string labelName, std::string editBoxDefaultText) {
	auto customBaseLayout = tgui::VerticalLayout::create();
	customBaseLayout->setSize("100%", "10%");
	layout->add(customBaseLayout);

	auto customBaseLabel = tgui::Label::create(labelName);
	customBaseLabel->setSize("100%", "40%");
	customBaseLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Left);
	customBaseLabel->getRenderer()->setTextColor(tgui::Color::White);
	customBaseLabel->getRenderer()->setTextSize(16);
	customBaseLayout->add(customBaseLabel);

	auto valorBaseEditBox = tgui::EditBox::create();
	valorBaseEditBox->setEnabled(false);
	valorBaseEditBox->setSize("100%", "60%");
	valorBaseEditBox->setDefaultText(editBoxDefaultText);
	valorBaseEditBox->getRenderer()->setOpacity(1.f);
	customBaseLayout->add(valorBaseEditBox);

	layout->addSpace(0.1f);

	return valorBaseEditBox;
}

int getEditBoxValue(tgui::EditBox::Ptr p) {
	try {
		return std::clamp(std::stoi(p->getText().toStdString()), 2, 35);
	}
	catch (const std::exception&) {
		return 2;  // Default to base 2 if invalid input
	}
}
int getEditBoxValueRaw(tgui::EditBox::Ptr p) {
	try {
		return  std::stoi(p->getText().toStdString());
	}
	catch (const std::exception&) {
		return -1;  // Default to base 2 if invalid input
	}
}
int getEditBoxValue(tgui::EditBox::Ptr p,int min, int max) {
	try {
		return std::clamp(std::stoi(p->getText().toStdString()), min, max);
	}
	catch (const std::exception&) {
		return 2;  // Default to base 2 if invalid input
	}
}
 

void changeValues() {
	std::string valor = valorTransformarPtr->getText().toStdString();
	int initialBase = getEditBoxValue(baseInputPtr);
	int targetBase = getEditBoxValue(transformBaseInputBoxPtr);
	int valorPrecision = getEditBoxValue(transformBaseInputPrecisionPtr,0,35);
	
	if (valor.empty()) return;

	try {
		std::string valorBinario = GeneralBaseConversion(valor, initialBase, 2, valorPrecision);
		std::string valorOctal = GeneralBaseConversion(valor, initialBase, 8, valorPrecision);
		std::string valorDecimal = GeneralBaseConversion(valor, initialBase, 10, valorPrecision);
		std::string valorHexadecimal = GeneralBaseConversion(valor, initialBase, 16, valorPrecision);
		std::string valorBaseCustom = GeneralBaseConversion(valor, initialBase, targetBase, valorPrecision);

		std::cout << valorBinario << std::endl;
		std::cout << valorOctal << std::endl;
		std::cout << valorDecimal << std::endl;
		std::cout << valorHexadecimal << std::endl;
		std::cout << valorBaseCustom << std::endl;


		valorBinarioPtr->setText(valorBinario);
		valorOctalPtr->setText(valorOctal);
		valorDecimalPtr->setText(valorDecimal);
		valorHexadecimalPtr->setText(valorHexadecimal);
		valorBaseInputPtr->setText(valorBaseCustom);
	}
	catch (const std::exception&) {
		// Handle invalid conversions
	}
}

int main() {
	sf::RenderWindow window(sf::VideoMode({ 500, 500 }), "Base N Calculator", sf::Style::Close);
	window.setFramerateLimit(60);



	
	
	sf::Texture backgroundTexture;
	std::filesystem::path p = std::filesystem::current_path();
	p+= "\\Assets\\images\\background.jpg";
	if (!backgroundTexture.loadFromFile(p)) {
		//std::cout << "Error loading background image" << std::endl;
	}
	sf::Sprite backgroundSprite(backgroundTexture);

	tgui::Gui gui(window);
	gui.setWindow(window);
	auto layout = tgui::VerticalLayout::create();
	layout->setSize("90%", "90%");
	layout->setPosition("5%", "5%");
	gui.add(layout);

	// Input for "Valor a transformar"
	auto valorTransformar = tgui::EditBox::create();
	valorTransformarPtr = valorTransformar;
	valorTransformar->setSize("100%", "8%");
	//valorTransformar->setMaximumCharacters(10);
	valorTransformar->setDefaultText("Valor a transformar");
	valorTransformar->getRenderer()->setBackgroundColor(tgui::Color::White);
	valorTransformar->getRenderer()->setTextColor(tgui::Color::Black);
	valorTransformar->getRenderer()->setPadding({ 10, 5, 10, 5 });
	layout->add(valorTransformar);

	layout->addSpace(0.05f);

	// Horizontal layout for Base inputs
	auto horizontalLayout = tgui::HorizontalLayout::create();
	horizontalLayout->setSize("100%", "12%");
	horizontalLayout->getRenderer()->setSpaceBetweenWidgets(10);
	layout->add(horizontalLayout);

	// Base Inicial Input
	auto verticalLayoutBaseInput = tgui::VerticalLayout::create();
	verticalLayoutBaseInput->setSize("40%", "100%");

	auto customBaseLabel = tgui::Label::create("Base Inicial");
	customBaseLabel->setSize("100%", "40%");
	customBaseLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Left);
	customBaseLabel->getRenderer()->setTextColor(tgui::Color::White);
	customBaseLabel->getRenderer()->setTextSize(16);
	verticalLayoutBaseInput->add(customBaseLabel);

	auto initialBaseInput = tgui::EditBox::create();
	initialBaseInput->setSize("100%", "60%");
	initialBaseInput->setDefaultText("Base (2-35)");
	initialBaseInput->getRenderer()->setBackgroundColor(tgui::Color::White);
	initialBaseInput->getRenderer()->setTextColor(tgui::Color::Black);
	initialBaseInput->getRenderer()->setPadding({ 5, 5, 5, 5 });
	baseInputPtr = initialBaseInput;
	verticalLayoutBaseInput->add(initialBaseInput);
	horizontalLayout->add(verticalLayoutBaseInput);

	// Arrow Label
	auto arrowLabel = tgui::Label::create("--->");
	arrowLabel->setSize("15%", "100%");
	arrowLabel->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
	arrowLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	arrowLabel->getRenderer()->setTextColor(tgui::Color::White);
	arrowLabel->getRenderer()->setTextSize(20);
	horizontalLayout->add(arrowLabel);

	// Nueva Base Input con Label
	auto verticalLayoutNuevaBase = tgui::VerticalLayout::create();
	verticalLayoutNuevaBase->setSize("40%", "100%");

	auto nuevaBaseLabel = tgui::Label::create("Nueva Base");
	nuevaBaseLabel->setSize("100%", "40%");
	nuevaBaseLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Left);
	nuevaBaseLabel->getRenderer()->setTextColor(tgui::Color::White);
	nuevaBaseLabel->getRenderer()->setTextSize(16);
	verticalLayoutNuevaBase->add(nuevaBaseLabel);

	auto nuevaBaseInput = tgui::EditBox::create();
	transformBaseInputBoxPtr = nuevaBaseInput;
	nuevaBaseInput->setSize("100%", "60%");
	nuevaBaseInput->setDefaultText("Nueva Base (2-35)");
	nuevaBaseInput->getRenderer()->setBackgroundColor(tgui::Color::White);
	nuevaBaseInput->getRenderer()->setTextColor(tgui::Color::Black);
	nuevaBaseInput->getRenderer()->setPadding({ 5, 5, 5, 5 });
	verticalLayoutNuevaBase->add(nuevaBaseInput);
	horizontalLayout->add(verticalLayoutNuevaBase);

	///// asldkjsldkads
	auto verticalLayoutNuevaBasePrecision = tgui::VerticalLayout::create();
	verticalLayoutNuevaBasePrecision->setSize("40%", "100%");

	auto nuevaBaseLabelPrecision = tgui::Label::create("Precision");
	nuevaBaseLabelPrecision->setSize("100%", "40%");
	nuevaBaseLabelPrecision->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Left);
	nuevaBaseLabelPrecision->getRenderer()->setTextColor(tgui::Color::White);
	nuevaBaseLabelPrecision->getRenderer()->setTextSize(16);
	verticalLayoutNuevaBasePrecision->add(nuevaBaseLabelPrecision);

	auto nuevaBaseInputPrecision = tgui::EditBox::create();
	transformBaseInputPrecisionPtr = nuevaBaseInputPrecision;
	nuevaBaseInputPrecision->setSize("100%", "60%");
	nuevaBaseInputPrecision->setDefaultText(" Valor precision (1-35)");
	nuevaBaseInputPrecision->getRenderer()->setBackgroundColor(tgui::Color::White);
	nuevaBaseInputPrecision->getRenderer()->setTextColor(tgui::Color::Black);
	nuevaBaseInputPrecision->getRenderer()->setPadding({ 5, 5, 5, 5 });
	verticalLayoutNuevaBasePrecision->add(nuevaBaseInputPrecision);

	horizontalLayout->add(verticalLayoutNuevaBasePrecision);



	layout->addSpace(0.1f);

	// Add Base Conversion Outputs
	valorBinarioPtr = addBaseToLayout(layout, "Base 2", "Valor en base 2");
	valorDecimalPtr = addBaseToLayout(layout, "Base 10", "Valor en base 10");
	valorOctalPtr = addBaseToLayout(layout, "Base 8", "Valor en base 8");
	valorHexadecimalPtr = addBaseToLayout(layout, "Base 16", "Valor en base 16");
	valorBaseInputPtr = addBaseToLayout(layout, "Base N", "Valor en Base N");
 
	// Events
	initialBaseInput->onTextChange([initialBaseInput, nuevaBaseInput]() {
		try {
			changeValues();
		}
		catch (const std::exception&) {
			// Handle exception
			//std::cout << "error en initial base input on text change"<<std::endl;
		}
		});
	initialBaseInput->onMouseLeave([initialBaseInput](){
		try {
			if (initialBaseInput->getText().empty() || !(getEditBoxValueRaw(initialBaseInput) >= 2 && getEditBoxValueRaw(initialBaseInput) <= 35)) {
				initialBaseInput->setText("2");
			}
		}
		catch (const std::exception&) {
			// Handle exception
			//std::cout << "error en initial base onMouseLeave" << std::endl;
		}
	});


	valorTransformar->onTextChange([]() {
		try {
			changeValues();
		}
		catch (const std::exception&) {
			// Handle exception
			//std::cout << "error en  valor transformar on text change" << std::endl;
		}
	});


	nuevaBaseInput->onTextChange([initialBaseInput, nuevaBaseInput]() {
		try {
			changeValues();
		}
		catch (const std::exception&) {
			// Handle exception
			std::cout << "error en  nueva base input on text change" << std::endl;
		}
		});
	nuevaBaseInput->onMouseLeave([nuevaBaseInput]() {
		try {
			if (nuevaBaseInput->getText().empty() || !(getEditBoxValueRaw(nuevaBaseInput) >= 2 && getEditBoxValueRaw(nuevaBaseInput) <= 35)) {
				nuevaBaseInput->setText("2");
			}
		}
		catch (const std::exception&) {
			// Handle exception
			//std::cout << "error en nuevaBaseInput  onMouseLeave" << std::endl;
		}
		});

	nuevaBaseInputPrecision->onTextChange( [nuevaBaseInputPrecision]() {
		try {
			changeValues();
		}
		catch (const std::exception&) {
			// Handle exception
			std::cout << "error en  nueva base input on text change" << std::endl;
		}
		});
	nuevaBaseInputPrecision->onMouseLeave([nuevaBaseInputPrecision]() {
		try {
			if (nuevaBaseInputPrecision->getText().empty() || !(getEditBoxValueRaw(nuevaBaseInputPrecision) >= 0 && getEditBoxValueRaw(nuevaBaseInputPrecision) <= 35)) {
				nuevaBaseInputPrecision->setText("0");
			}
		}
		catch (const std::exception&) {
			// Handle exception
			//std::cout << "error en nuevaBaseInput  onMouseLeave" << std::endl;
		}
		});

	// Main loop
	while (window.isOpen()) {
		while (auto event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>())
				window.close();

			gui.handleEvent(*event);
		}

		window.clear();
		window.draw(backgroundSprite);
		gui.draw();
		window.display();
	}

	return 0;
}
