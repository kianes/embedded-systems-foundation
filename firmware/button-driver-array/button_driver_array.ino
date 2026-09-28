#include <Arduino.h>

const uint8_t LED_PIN = 8;
const uint8_t BUTTON_COUNT = 2;

enum ButtonState
{
    RELEASED,
    PRESS_DEBOUNCE,
    PRESSED,
    RELEASE_DEBOUNCE
};

struct Button
{
    uint8_t pin;
    ButtonState state;
    unsigned long debounceStart;
    bool stablePressed;
    bool pressEvent;
};

Button buttons[BUTTON_COUNT] =
{
    {7, RELEASED, 0, false, false},
    {6, RELEASED, 0, false, false}
};

bool ledState = false;

void updateButton(Button &button)
{
    button.pressEvent = false;

    bool pressed = (digitalRead(button.pin) == LOW);

    if (button.state == RELEASED)
    {
        if (pressed)
        {
            button.state = PRESS_DEBOUNCE;
            button.debounceStart = millis();
        }
    }

    else if (button.state == PRESS_DEBOUNCE)
    {
        if (pressed)
        {
            if (millis() - button.debounceStart >= 5)
            {
                button.state = PRESSED;
                button.stablePressed = true;
                button.pressEvent = true;
            }
        }
        else
        {
            button.state = RELEASED;
        }
    }

    else if (button.state == PRESSED)
    {
        if (!pressed)
        {
            button.state = RELEASE_DEBOUNCE;
            button.debounceStart = millis();
        }
    }

    else if (button.state == RELEASE_DEBOUNCE)
    {
        if (!pressed)
        {
            if (millis() - button.debounceStart >= 5)
            {
                button.state = RELEASED;
                button.stablePressed = false;
            }
        }
        else
        {
            button.state = PRESSED;
        }
    }
}

void setup()
{
    pinMode(LED_PIN, OUTPUT);

    for (uint8_t i = 0; i < BUTTON_COUNT; i++)
    {
        pinMode(buttons[i].pin, INPUT_PULLUP);
    }
}

void loop()
{
    for (uint8_t i = 0; i < BUTTON_COUNT; i++)
    {
        updateButton(buttons[i]);
    }

    if (buttons[0].pressEvent)
    {
        ledState = true;
    }

    if (buttons[1].pressEvent)
    {
        ledState = false;
    }

    digitalWrite(LED_PIN, ledState);
}
