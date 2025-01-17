#include "selection.h"
#include "gui/menu_item/menu_item.h"
#include "gui/ui/sound_editor.h"
#include "hid/display/display.h"
#include "hid/display/oled.h"

namespace deluge::gui::menu_item {
void Selection::drawValue() {
	if (display->haveOLED()) {
		renderUIsForOled();
	}
	if (display->have7SEG()) {
		const auto options = this->getOptions();
		display->setScrollingText(options[this->getValue()].data());
	}
}

void Selection::drawPixelsForOled() {
	auto value = this->getValue();
	// Move scroll
	if (soundEditor.menuCurrentScroll > value) {
		soundEditor.menuCurrentScroll = value;
	}
	else if (soundEditor.menuCurrentScroll < value - kOLEDMenuNumOptionsVisible + 1) {
		soundEditor.menuCurrentScroll = value - kOLEDMenuNumOptionsVisible + 1;
	}

	const int32_t selectedOption = value - soundEditor.menuCurrentScroll;

	MenuItem::drawItemsForOled(std::span{getOptions().data(), getOptions().size()}, selectedOption,
	                           soundEditor.menuCurrentScroll);
}

// renders check box on OLED and dot on 7seg
void Selection::displayToggleValue() {
	if (display->haveOLED()) {
		renderUIsForOled();
	}
	else {
		drawName();
	}
}

// handles rendering of the community features menu items that are identified as toggles
void Selection::renderSubmenuItemTypeForOled(int32_t yPixel) {
	deluge::hid::display::oled_canvas::Canvas& image = deluge::hid::display::OLED::main;

	int32_t startX = getSubmenuItemTypeRenderIconStart();

	if (isToggle()) {
		if (getToggleValue()) {
			image.drawGraphicMultiLine(deluge::hid::display::OLED::checkedBoxIcon, startX, yPixel, 7);
		}
		else {
			image.drawGraphicMultiLine(deluge::hid::display::OLED::uncheckedBoxIcon, startX, yPixel, 7);
		}
	}
	else {
		image.drawGraphicMultiLine(deluge::hid::display::OLED::submenuArrowIcon, startX, yPixel, 7);
	}
}

} // namespace deluge::gui::menu_item
