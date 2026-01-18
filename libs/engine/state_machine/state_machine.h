// state_machine.h

#pragma once

// =========================================================================================== IMPORT

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <functional>
#include <memory>

#include "../platform/platform.h"

// =========================================================================================== IMPORT


// =========================================================================================== STATE_ID



/**
 * @brief Represents a hierarchical identifier for a game state.
 *
 * Each State_ID consists of a sequence of integers defining its
 * position in a state hierarchy. Useful for state machines in games
 * or applications where states can have nested sub-states.
 *
 * Example usage:
 * @code State_ID game = {1, 1}; @endcode
 * @code State_ID small_menu = game.child(2);  // Results in {1, 1, 2} @endcode
 */
class State_ID
{

public:

    // Sequence of integers representing hierarchical levels.
    std::vector<int> levels;

    // Default constructor, creates an empty State_ID.
    State_ID() = default;

    /**
     * @brief Construct a State_ID from a list of levels.
     *
     * @param lvl Initializer list of integers representing levels.
     */
    State_ID(std::initializer_list<int> lvl);


    /**
     * @brief Equality operator.
     *
     * @param other Another State_ID to compare with.
     * @return true if both State_IDs have identical levels, false otherwise.
     */
    bool operator==(const State_ID& other) const;

    /**
     * @brief Inequality operator.
     *
     * @param other Another State_ID to compare with.
     * @return true if the State_IDs differ, false if they are equal.
     */
    bool operator!=(const State_ID& other) const;


    /**
     * @brief Returns the parent State_ID in the hierarchy.
     *
     * Removes the last level from the current State_ID. If the State_ID
     * is empty, returns an empty State_ID.
     *
     * @return State_ID of the parent.
     */
    State_ID parent() const;

    /**
     * @brief Checks if this State_ID is the parent of another.
     *
     * A State_ID is considered the parent if the other State_ID has
     * exactly one more level and all previous levels match.
     * 
     * Logic:
     * 
     * - Child must have exactly one more level than this State_ID.
     * 
     * - All preceding levels must match.
     * 
     * This guarantees a proper hierarchical parent-child relationship.
     *
     * @param child The candidate child State_ID.
     * @return true if this is the parent of child, false otherwise.
     */
    bool is_parent_of(const State_ID& child) const;


    /**
     * @brief Returns a new State_ID as a child of this one.
     *
     * Adds the given integer as a new level at the end of the current
     * levels to form a hierarchical child.
     *
     * @param i The level index for the new child.
     * @return State_ID representing the child.
     */
    State_ID child(int i) const;


        /**
     * @brief Converts the State_ID to a dot-separated string.
     *
     * Example:
     * 
     * State_ID id = {1, 2, 3};
     * 
     * std::string s = id.string(); // "1.2.3"
     *
     * @return std::string representation of the State_ID.
     */
    std::string string() const;
};

// =========================================================================================== STATE_ID


// =========================================================================================== STATE


/**
 * @brief Represents a single state in a hierarchical state machine.
 *
 * Each State can have a parent and multiple children, forming a tree structure.
 * States can define callbacks for entering, exiting, and updating the state.
 *
 * Example usage:
 * 
 * State game({1,1}, "GAME");
 * 
 * game.on_enter = [](){ std::cout << "Entered GAME state\n"; };
 * 
 * State small_menu = game.child(2); // child example
 * 
 */
class State
{

public:

    // Unique hierarchical identifier for this state.
    State_ID id;

    // Human-readable name of the state.
    std::string name;

    // Callback executed when entering this state.
    std::function<void()> on_enter;

    // Callback executed when exiting this state.
    std::function<void()> on_exit;

    // Callback executed every update tick while in this state for event handling 
    std::function<void(SDL_Event&)> state_handle_event;

    // Callback executed every update tick while in this state for state apps update operations.
    std::function<void()> state_update;

    // Callback executed every update tick while in this state for state rendering operation
    std::function<void(SDL_Renderer*)> state_render;

    // Pointer to the parent state. nullptr if this is a root state.
    State* parent = nullptr;

    // Pointers to child states of this state.
    std::vector<State*> children;


    /**
     * @brief Constructor to create a state with an ID and name.
     *
     * @param i Hierarchical State_ID for this state.
     * @param n Human-readable name of the state.
     */
    State(const State_ID& i, const std::string& n);

    /**
     * @brief Destructor.
     *
     * Automatically clears children and callbacks if needed.
     */
    ~State();
};

// =========================================================================================== STATE


// =========================================================================================== STATE MACHINE


/**
 * @brief Manages a collection of States and handles transitions between them.
 *
 * The State_machine stores states as unique pointers and keeps track of
 * the currently active state. It supports adding new states, switching
 * between states, and updating the current state.
 *
 * Example usage:
 * 
 * State_machine sm;
 * 
 * sm.add_state(std::make_unique<State>(State_ID{1}, "START"));
 * 
 * sm.go_to(State_ID{1});
 * 
 * sm.update(); // Calls update callback of the current state
 * 
 */
class State_machine
{

private:

    // Container of all states managed by this machine.
    std::vector<std::unique_ptr<State>> states;

    // Pointer to the currently active state.
    State* current_state = nullptr;
    
    /**
     * @brief Checks if a State_ID already exists in the machine.
     *
     * @param id State_ID to check.
     * @return true if a state with the given ID exists, false otherwise.
     */
    bool id_exists(const State_ID& id) const;

public:

    // Default constructor. 
    State_machine() = default;
    // Default destructor.
    ~State_machine() = default;


    /**
     * @brief Adds a new state to the state machine.
     *
     * @param s Unique pointer to the State to add.
     * @return true if the state was added successfully, false if a state
     *         with the same ID already exists.
     */
    bool add_state(std::unique_ptr<State> s);


    /**
     * @brief Switches the machine to a state with the given ID.
     *
     * Calls on_exit of the current state and on_enter of the new state.
     *
     * @param id The State_ID to switch to.
     * @return true if the transition was successful, false if the ID was not found.
     */
    bool go_to(const State_ID& id);


    /**
     * @brief Returns a pointer to the currently active state.
     *
     * @return Pointer to the current State, or nullptr if no state is active.
     */
    State* get_current_state() const;


    /**
     * @brief Returns the name of the currently active state.
     *
     * If no state is active, returns "NONE".
     *
     * @return std::string Name of the current state or "NONE" if no state is active.
     */
    std::string current_state_name() const;


    /**
     * @brief Passes an SDL event to the currently active state.
     *
     * Any SDL event (keyboard, mouse, controller button, etc.) is forwarded.
     * Each state can decide how to handle it:
     * 
     * - MAIN_MENU might wait for Enter or navigation keys.
     * 
     * - GAME might handle character movement, shooting, or other gameplay input.
     *
     * @param e Reference to the SDL_Event to handle.
     */
    void state_handle_event(SDL_Event& e);


    /**
     * @brief Delegates rendering to the currently active state.
     *
     * Each state knows how to draw itself: menus, game objects, UI elements, text, etc.
     * The SDL_Renderer pointer is passed down so states can draw directly to the screen.
     *
     * @param r Pointer to the SDL_Renderer used for drawing.
     */
    void state_render(SDL_Renderer* r);


    // Updates the logic of the current state.
    // Only the current state is updated; parent or sibling states are ignored.
    // This keeps the update loop simple and local to the active state.
    void state_update();
};

// =========================================================================================== STATE MACHINE
