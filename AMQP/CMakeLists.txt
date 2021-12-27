
set(This AMQP)

set(Headers
        include/Channel.hpp
        )

set(Sources
        src/Channel.cpp
        )

include_directories(../rabbitmq-c/include)

add_library(${This} SHARED ${Sources} ${Headers})

target_include_directories(${This} PUBLIC include)

set_target_properties(${This} PROPERTIES
        FOLDER Libraries
        )

target_link_libraries(${This} rabbitmq)
