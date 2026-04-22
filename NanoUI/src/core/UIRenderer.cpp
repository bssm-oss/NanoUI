#include "UIRenderer.h"

namespace NanoUI {

void UIRenderer::renderScreen(UIScreen* screen) {
    if (!_driver || !screen) return;

    _driver->fillScreen(screen->backgroundColor);

    for (uint8_t i = 0; i < screen->componentCount; i++) {
        UIComponent* comp = screen->components[i];
        if (comp && comp->visible) {
            comp->needsRedraw = true;
            comp->draw(_driver);
            comp->needsRedraw = false;
        }
    }
}

void UIRenderer::renderComponent(UIComponent* comp) {
    if (!_driver || !comp || !comp->visible) return;
    comp->needsRedraw = true;
    comp->draw(_driver);
    comp->needsRedraw = false;
}

} // namespace NanoUI
