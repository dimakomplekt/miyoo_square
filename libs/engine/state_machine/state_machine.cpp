// state_machine.cpp

// =========================================================================================== IMPORT

#include "state_machine.h"

#include <algorithm> // For "std::find_if" and "std::remove"

// =========================================================================================== IMPORT


// =========================================================================================== STATE_ID

// We initialize the empty internal vector directly from the initializer list.

State_ID::State_ID(std::initializer_list<int> lvl) : levels(lvl) {}


// Vector equality already checks both size and each element in order.

bool State_ID::operator==(const State_ID &other) const { return levels == other.levels; }


// Implemented in terms of operator== to avoid duplicating logic.

bool State_ID::operator!=(const State_ID &other) const { return !(*this == other); }


State_ID State_ID::parent() const
{
    // We make a copy of the current object to avoid modifying the original.
    // Then we remove the last level, if it exists. This works even if the
    // current State_ID is empty (will return another empty State_ID).

    State_ID parent_copy = *this;

    // // Remove last level if there is a level
    if (!parent_copy.levels.empty())
        parent_copy.levels.pop_back();

    return parent_copy; // Copy as parent
}


bool State_ID::is_parent_of(const State_ID &child) const
{
    // Quick fail if size mismatch
    if (levels.size() + 1 != child.levels.size())
        return false;

    // Compare each level
    for (size_t i = 0; i < levels.size(); ++i)

        if (levels[i] != child.levels[i])
            return false; // Mismatch found

    return true; // All levels match
}


// We copy the current object to avoid modifying it, then push_back
// the new level. This allows chaining calls like: game.child(2).child

State_ID State_ID::child(int i) const
{
    State_ID curr = *this;    // Copy current
    curr.levels.push_back(i); // Add new child level
    return curr;              // Return new object
}


// We iterate over all levels and insert dots between them.
// This creates a human-readable hierarchical representation.

std::string State_ID::string() const
{
    // Concatenation buffer
    std::ostringstream oss;

    for (size_t i = 0; i < levels.size(); ++i)
    {
        if (i > 0) oss << ".";   // Insert dot between levels
        oss << levels[i]; // Add level number
    }

    return oss.str(); // Buffered string return
}

// =========================================================================================== STATE_ID


// =========================================================================================== STATE

// Initializes the state with a hierarchical ID and human-readable name.

// Pass without copy by & and guarantees data security by const

// All callbacks are set to nullptr by default to indicate that no action
// occurs unless explicitly assigned.
// The parent pointer is set to nullptr, meaning the state is initially
// considered a root until added to a hierarchy.

// This design allows safe creation of states without requiring all callbacks
// to be assigned immediately.

State::State(const State_ID &state_id, const std::string &state_name)
    : id(state_id), name(state_name), on_enter(nullptr), on_exit(nullptr), state_update(nullptr),
      state_handle_event(nullptr), state_render(nullptr), parent(nullptr) {}


// Default destructor is sufficient because:
// We do not own child states (they are managed externally, e.g., by State_machine)
// std::function automatically cleans up any assigned callable objects
// std::vector cleans up its own memory

State::~State() = default;

// =========================================================================================== STATE


// =========================================================================================== STATE MACHINE

bool State_machine::add_state(std::unique_ptr<State> s)
{
    // Reject if a state with the same ID already exists
    if (id_exists(s->id))
    {
        std::cerr << "State with ID " << s->id.string() << " already exists!\n";
        return false;
    }

    State *raw = s.get(); // Keep raw pointer for hierarchy linking

    // 1. Find the parent (if it exists) and attach this state as a child
    for (auto &existing : states)
    {
        if (existing->id == s->id.parent())
        {
            raw->parent = existing.get();
            existing->children.push_back(raw);
            break;
        }
    }

    // 2. Find any existing children that this new state should adopt
    for (auto &existing : states)
    {
        if (s->id.is_parent_of(existing->id))
        {
            existing->parent = raw;
            raw->children.push_back(existing.get());
        }
    }

    // Finally, store the state in the machine
    states.push_back(std::move(s));
    return true;
}

bool State_machine::id_exists(const State_ID &id) const
{
    // Iterate through all states to check if the given ID already exists
    for (const auto &s : states) if (s->id == id) return true;

    return false; // Not found
}

// Initialization by the smart pointer which will deallocate automatically

void State_machine::initiate_state(const State_ID &state_id, const std::string &state_name)
{
    auto new_state = std::make_unique<State>(state_id, state_name); // Create state's smart pointer

    this->add_state(std::move(new_state)); // Add state to the state machine

    // Memory for the state will deallocate automatically
}


// State getter by ID from state machine

State *State_machine::get_state(const State_ID &state_id)
{
    // Iterative search of the state by ID
    for (auto &s : this->states) if (s->id == state_id) return s.get(); // Smart pointer return

    return nullptr; // No state by state_id case
}

// Helper-function for recursive removing of the state childrens
// and nullptring of the deleted state parent children pointer.
// Called inside the clear_state(const State_ID& id);
void remove_state_recursive(State *s, std::vector<std::unique_ptr<State>> &states)
{
    // Recursive childrens removing by the std::vector<State*> children container
    for (State *child : s->children) remove_state_recursive(child, states);

    // Remove the state from the state parent's std::vector<State*> children container
    if (s->parent)
    {
        // Childrens of the deleted state parent
        auto &siblings = s->parent->children;

        // Move to the left all the elements which are not equal to s to the begining
        // and erase the s element from the vector
        siblings.erase(std::remove(siblings.begin(), siblings.end(), s), siblings.end());
    }

    // Delete the element from the state machine states vector

    // "[&](const std::unique_ptr<State>& sp) { return sp.get() == s; }" is a predicate lambda
    // used by STL algorithms (like std::find_if or std::remove_if).
    // Breakdown:
    // - [&] : capture list by reference, allows the lambda to use external variables (here, 's').
    // - (const std::unique_ptr<State>& sp) : each element of the container (a unique_ptr<State>) is passed in.
    // - return sp.get() == s : returns true if the raw pointer inside the unique_ptr matches the target pointer 's'.
    //   Essentially, it checks whether this is the specific state we want to find or remove.
    auto it = std::find_if(

        states.begin(), states.end(),

        [&](const std::unique_ptr<State> &sp)

        { return sp.get() == s; }

    );

    if (it != states.end()) states.erase(it); // unique_ptr will call destructor automatically
}

void State_machine::clear_state(const State_ID &id)
{
    // Find the state pointer
    auto it = std::find_if(

        states.begin(), states.end(),

        [&](const std::unique_ptr<State> &s)

        { return s->id == id; }

    );

    if (it == states.end()) return; // If there is no state with passed ID - do nothing


    // Get the deleted state pointer by the unique_ptr.h function
    State *target = it->get();

    // If the deleted state is current - call an on_exit callback
    // and nullptr current state
    if (current_state && current_state == target)
    {
        if (current_state->on_exit) current_state->on_exit();
        current_state = nullptr;
    }


    // Recursive state clear
    remove_state_recursive(target, states);
}

void State_machine::clear_states()
{
    // Nullptr all hierarchy links
    for (auto &s : states)
    {
        if (s->parent)
        {
            auto &siblings = s->parent->children;

            // std::remove_if idiom:
            // - std::remove moves all element != s.get(), into the vector begining
            // - elements, whose are == s.get() (current state ptr), moves to the end
            // - std::remove returns the iterator to the new logical ending of the vector limits
            // - erase deletes this elements from the container

            siblings.erase(

                std::remove(siblings.begin(), siblings.end(), s.get()),
                siblings.end()

            );
        }

        s->children.clear(); // Remove the links on the childrens
    }

    // Clear an unique pointer
    states.clear();

    // Nullptr current state
    current_state = nullptr;
}

bool State_machine::go_to(const State_ID &id)
{
    // Search for the target state by ID
    for (auto &s : states)
    {
        // Call exit callback on current state if exists
        if (s->id == id)
        {
            if (current_state && current_state->on_exit) current_state->on_exit();

            current_state = s.get(); // Switch to the new state

            // Call enter callback on new state if exists
            if (current_state->on_enter) current_state->on_enter();

            
            return true;
        }
    }

    // State not found handler
    std::cerr << "State not found: " << id.string() << "\n";

    return false;
}


// Simply return pointer to the currently active state

State *State_machine::get_current_state() const { return current_state; }


// Using function pointer (std::function) in State allows flexible per-state behavior.

void State_machine::state_handle_event(SDL_Event &e)
{
    if (current_state && current_state->state_handle_event) current_state->state_handle_event(e);
}


// Delegates all rendering to the current state.
// Each state knows how to draw itself: menus, game objects, UI elements, text, etc.
// The renderer is passed down so states can draw directly to the screen.

void State_machine::state_render(SDL_Renderer *r)
{
    if (current_state && current_state->state_render) current_state->state_render(r);
}


// Updates the logic of the current state.
// Only the current state is updated; parent or sibling states are ignored.
// This keeps the update loop simple and local to the active state.

void State_machine::state_update()
{
    if (current_state && current_state->state_update) current_state->state_update();
}


// Returns the human-readable name of the current state.
// If no state is active, returns "NONE".
// Useful for debugging, logging, or conditional logic outside the state machine.

std::string State_machine::current_state_name() const
{
    return current_state ? current_state->name : "NONE";
}

// =========================================================================================== STATE MACHINE
