#ifndef CONTROL_PANEL_H
#define CONTROL_PANEL_H

namespace controlPanel
{
    constexpr int selectButtonPin{11};
    constexpr int prevButtonPin{12};
    constexpr int nextButtonPin{13};

    constexpr long debounceDelay{500};
}

using namespace controlPanel;

class ControlPanel
{
private:
    struct Button
    {
        bool pressed{};
        unsigned long lastPressTime{};
        int pin;

        Button(int p) : pin(p) {}
    };

    Button m_selectButton{selectButtonPin};
    Button m_prevButton{prevButtonPin};
    Button m_nextButton{nextButtonPin};

    void checkButton(Button &button);

public:
    ControlPanel();
    void cycle();

    // Getter methods to check button states
    bool isSelect() const;
    bool isPrev() const;
    bool isNext() const;
};

#endif