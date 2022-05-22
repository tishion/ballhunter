project(third-party)

#################################################
file(GLOB_RECURSE easyloggingpp_SRC_FILES
    "./easylogging++/*.h"
    "./easylogging++/*.cc"
)
add_library(easyloggingpp STATIC 
    ${easyloggingpp_SRC_FILES}
)

set_target_properties(easyloggingpp 
    PROPERTIES 
        FOLDER third-party
)

add_subdirectory(reactphysics3d)
set_target_properties(reactphysics3d 
    PROPERTIES 
        FOLDER third-party
)