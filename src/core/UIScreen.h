#ifndef UI_SCREEN_H
#define UI_SCREEN_H

#include <cstring>
#include "../components/UIComponent.h"

namespace NanoUI {

/**
 * @brief Maximum length for screen ID strings
 */
#define MAX_SCREEN_ID_LEN 16

/**
 * @brief Represents a single screen/page in the UI
 * 
 * A screen contains a background color and a collection of UI components.
 * Only one screen is visible at a time.
 */
class UIScreen {
public:
    char id[MAX_SCREEN_ID_LEN];
    uint16_t backgroundColor;
    UIComponent* components[MAX_COMPONENTS_PER_SCREEN];
    uint8_t componentCount;

    UIScreen() : backgroundColor(0x0000), componentCount(0) {
        id[0] = '\0';
        for (int i = 0; i < MAX_COMPONENTS_PER_SCREEN; i++) {
            components[i] = nullptr;
        }
    }

    ~UIScreen() {
        for (int i = 0; i < componentCount; i++) {
            if (components[i]) {
                delete components[i];
                components[i] = nullptr;
            }
        }
    }

    /**
     * @brief Set the screen ID
     * @param screenId ID string
     */
    void setId(const char* screenId) {
        if (screenId) {
            strncpy(id, screenId, MAX_SCREEN_ID_LEN - 1);
            id[MAX_SCREEN_ID_LEN - 1] = '\0';
        }
    }

    /**
     * @brief Add a component to this screen
     * @param comp Pointer to the component (ownership transferred)
     */
    void addComponent(UIComponent* comp) {
        if (componentCount < MAX_COMPONENTS_PER_SCREEN && comp) {
            components[componentCount++] = comp;
        }
    }

    /**
     * @brief Find a component by its ID
     * @param compId Component ID string
     * @return Pointer to the component, or nullptr if not found
     */
    UIComponent* findComponent(const char* compId) const {
        for (uint8_t i = 0; i < componentCount; i++) {
            if (components[i] && strcmp(components[i]->id, compId) == 0) {
                return components[i];
            }
        }
        return nullptr;
    }
};

} // namespace NanoUI

#endif // UI_SCREEN_H
