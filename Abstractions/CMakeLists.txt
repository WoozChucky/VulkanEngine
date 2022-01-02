
set(This Abstractions)

set(Headers
        include/Abstractions/DataTypes.hpp
        include/Abstractions/Logger.hpp
        include/Abstractions/LogCategory.hpp
        include/Abstractions/Environment.hpp
        include/Abstractions/Utils.hpp
        )

set(Sources
        src/Logger.cpp
        src/LogCategory.cpp
        src/Environment.cpp
        )

add_library(${This} STATIC ${Sources} ${Headers})

target_include_directories(${This} PUBLIC include)

set_target_properties(${This} PROPERTIES
        FOLDER Libraries
        )

target_link_libraries(${This})
