#include "controlPanel.h"
#include <Arduino.h>

using namespace controlPanel;

// Initialize the buttons
ControlPanel::ControlPanel() {}

void ControlPanel::checkButton(Button &button)
{
    if (digitalRead(button.pin) == HIGH)
    {
        if (!button.pressed)
        {
            if (millis() - button.lastPressTime > debounceDelay)
            {
                button.pressed = true;
                button.lastPressTime = millis();
            }
        }
    }
    else
    {
        button.pressed = false;
    }
}

void ControlPanel::cycle()
{
    checkButton(m_selectButton);
    checkButton(m_prevButton);
    checkButton(m_nextButton);
}

// Getter methods to check button states
bool ControlPanel::isSelect() const
{
    return m_selectButton.pressed;
}

bool ControlPanel::isPrev() const
{
    return m_prevButton.pressed;
}

bool ControlPanel::isNext() const
{
    return m_nextButton.pressed;
}