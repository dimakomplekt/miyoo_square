// state_machine.cpp


// =========================================================================================== IMPORT

#include "state_machine.h"

// =========================================================================================== IMPORT


// =========================================================================================== STATE_ID


// We initialize the empty internal vector directly from the initializer list.

State_ID::State_ID(std::initializer_list<int> lvl) : levels(lvl) {}


// Vector equality already checks both size and each element in order.

bool State_ID::operator==(const State_ID& other) const { return levels == other.levels; }

// Implemented in terms of operator== to avoid duplicating logic.

bool State_ID::operator!=(const State_ID& other) const { return !(*this == other); }



State_ID State_ID::parent() const
{
    // We make a copy of the current object to avoid modifying the original.
    // Then we remove the last level, if it exists. This works even if the
    // current State_ID is empty (will return another empty State_ID).

    State_ID parent_copy = *this;

    // // Remove last level if there is a level
    if (!parent_copy.levels.empty()) parent_copy.levels.pop_back();

    return parent_copy; // Copy as parent
}



bool State_ID::is_parent_of(const State_ID& child) const
{
     // Quick fail if size mismatch
    if (levels.size() + 1 != child.levels.size()) return false;

    // Compare each level
    for (size_t i = 0; i < levels.size(); ++i)
    
        if (levels[i] != child.levels[i]) return false; // Mismatch found

    return true; // All levels match
}


// We copy the current object to avoid modifying it, then push_back
// the new level. This allows chaining calls like: game.child(2).child

State_ID State_ID::child(int i) const
{
    State_ID curr = *this;          // Copy current
    curr.levels.push_back(i);       // Add new child level
    return curr;                    // Return new object
}


// We iterate over all levels and insert dots between them.
// This creates a human-readable hierarchical representation.

std::string State_ID::string() const
{
    // Concatenation buffer
    std::ostringstream oss;

    for (size_t i = 0; i < levels.size(); ++i)
    {
        if (i > 0) oss << ".";      // Insert dot between levels
        oss << levels[i];           // Add level number
    }
    return oss.str(); // Buffered string return
}

// =========================================================================================== STATE_ID


// =========================================================================================== STATE

// Initializes the state with a hierarchical ID and human-readable name.

// All callbacks are set to nullptr by default to indicate that no action
// occurs unless explicitly assigned.
// The parent pointer is set to nullptr, meaning the state is initially
// considered a root until added to a hierarchy.

// This design allows safe creation of states without requiring all callbacks
// to be assigned immediately.

State::State(const State_ID& i, const std::string& n)
    : id(i), name(n), on_enter(nullptr), on_exit(nullptr), state_update(nullptr),
    state_handle_event(nullptr), state_render(nullptr), parent(nullptr) {}


// Default destructor is sufficient because:
// We do not own child states (they are managed externally, e.g., by State_machine)
// std::function automatically cleans up any assigned callable objects
// std::vector cleans up its own memory

State::~State() = default;

// =========================================================================================== STATE


// =========================================================================================== STATE MACHINE


bool State_machine::id_exists(const State_ID& id) const
{
    // Iterate through all states to check if the given ID already exists
    for (const auto& s : states) if (s->id == id) return true;

    return false; // Not found
}



bool State_machine::add_state(std::unique_ptr<State> s)
{
    // Reject if a state with the same ID already exists
    if (id_exists(s->id))
    {
        std::cerr << "State with ID " << s->id.string() << " already exists!\n";
        return false;
    }

    State* raw = s.get(); // Keep raw pointer for hierarchy linking

    // 1. Find the parent (if it exists) and attach this state as a child
    for (auto& existing : states)
    {
        if (existing->id == s->id.parent())
        {
            raw->parent = existing.get();
            existing->children.push_back(raw);
            break;
        }
    }

    // 2. Find any existing children that this new state should adopt
    for (auto& existing : states)
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


bool State_machine::go_to(const State_ID& id)
{
    // Search for the target state by ID
    for (auto& s : states)
    {
        // Call exit callback on current state if exists
        if (s->id == id)
        {
            if (current_state && current_state->on_exit) current_state->on_exit();

            current_state = s.get();  // Switch to the new state

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

State* State_machine::get_current_state() const { return current_state; }



// Using function pointer (std::function) in State allows flexible per-state behavior.

void State_machine::state_handle_event(SDL_Event& e) {

    if(current_state && current_state->state_handle_event) current_state->state_handle_event(e);
}


// Delegates all rendering to the current state.
// Each state knows how to draw itself: menus, game objects, UI elements, text, etc.
// The renderer is passed down so states can draw directly to the screen.

void State_machine::state_render(SDL_Renderer* r) {

    if(current_state && current_state->state_render) current_state->state_render(r);
}


// Updates the logic of the current state.
// Only the current state is updated; parent or sibling states are ignored.
// This keeps the update loop simple and local to the active state.

void State_machine::state_update()
{
    if(current_state && current_state->state_update) current_state->state_update();
}


// Returns the human-readable name of the current state.
// If no state is active, returns "NONE".
// Useful for debugging, logging, or conditional logic outside the state machine.

std::string State_machine::current_state_name() const
{
    return current_state ? current_state->name : "NONE";
}

// =========================================================================================== STATE MACHINE
