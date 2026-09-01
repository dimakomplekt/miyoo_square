// instance_manager.h

#pragma once


// =========================================================================================== IMPORT


#include "instance.h"

// =========================================================================================== IMPORT


// =========================================================================================== HELPERS



// Instance slot ctx
struct asset_slot_ctx {

    Instance* instance;           // Instance

    handle_ctx handle;            // Instance handle (index and generation)

};
