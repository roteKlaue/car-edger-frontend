#pragma once

#include <functional>
#include "Component.h"

/**
 * @brief Represents a push button GUI component.
 *
 * This class extends the Component base class to provide a simple push button
 * with optional text and click behavior. The button can be displayed inside
 * a Frame and responds to standard Windows button messages.
 */
class Button :
    public Component
{
public:
    /**
     * @brief Constructs a default button with the label "Button".
     */
    Button();
    
    /**
     * @brief Constructs a button with the specified text label.
     * @param text The label to display on the button.
     */
    Button(std::wstring text);
    
    /// Deleted copy constructor.
    Button(const Button&) = delete;
    
    /// Deleted copy assignment operator.
    Button& operator=(const Button&) = delete;

    /**
     * @brief Gets the current text of the button.
     * @return The button label.
     */
    void SetText(const std::wstring& text) override;

    /**
     * @brief Destroys the native Windows button control.
     */
    void Destroy() override;
    
    /**
     * @brief Gets the current text of the button.
     * @return The button label.
     */
    std::wstring GetText() const override { return text; };

    /**
     * @brief Sets the callback function to be invoked on click.
     * @param callback Function to be called when the button is clicked.
     */
    void SetOnClick(std::function<void()> callback);

	/**
	 * @brief Gets the current click callback function.
	 * @return The function to be called when the button is clicked.
	 */
    std::function<void()> GetOnClick() const;

	/**
	 * @brief Creates the button control in the parent window.
	 * Initializes the button with the specified text and styles.
	 */
    void Create() override;

private:
    std::wstring text;                    ///< Text displayed on the button.
    std::function<void()> onClick;        ///< Callback function for click events.


    /**
     * @brief Handles WM_COMMAND messages related to this button.
     *
     * Called automatically when the parent window receives a command event.
     * @param wParam Contains the notification code and control ID.
     * @param lParam Handle to the control that sent the message.
     */
    void HandleCommand(WPARAM wParam, LPARAM lParam) override;
};
