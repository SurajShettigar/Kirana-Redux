message("Compiling Shaders...")

set(SHADERS_ROOT_DIR_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
set(SHADERS_ROOT_DIR_COMPILED ${CMAKE_CURRENT_BINARY_DIR}/shaders)

add_custom_target(Shaders ALL)

function(addShader SOURCE_FILE SHADER_DEPENDENCIES)
    if (NOT EXISTS ${SOURCE_FILE})
        message(SEND_ERROR "Shader file does not exist: " ${SOURCE_FILE})
        return()
    endif ()

    # Parse optional arguments:
    # 0 - Shader Type (one of vert, frag, comp, etc.)
    # 1-N - List of entry point names present in the given shader file.
    if (ARGN)
        list(LENGTH ARGN NUM_ARGS)
        if (${NUM_ARGS} GREATER 1)
            list(SUBLIST ARGN 0 ${NUM_ARGS} SHADER_ENTRIES)
        endif ()
    endif ()

    # Get shader sub-folder if it exists and shader file name.
    cmake_path(RELATIVE_PATH SOURCE_FILE BASE_DIRECTORY ${SHADERS_ROOT_DIR_SOURCE} OUTPUT_VARIABLE SOURCE_FILE_RELATIVE)
    get_filename_component(SOURCE_FILE_FOLDER ${SOURCE_FILE_RELATIVE} DIRECTORY)
    cmake_path(GET SOURCE_FILE STEM LAST_ONLY SOURCE_FILE_NAME)

    # Create the folder hierarchy for the compiled shader.
    if (NOT ${SOURCE_FILE_FOLDER} STREQUAL "")
        set(COMPILED_FILE_PARENT ${SHADERS_ROOT_DIR_COMPILED}/${SOURCE_FILE_FOLDER})
    else ()
        set(COMPILED_FILE_PARENT ${SHADERS_ROOT_DIR_COMPILED})
    endif ()
    if (NOT EXISTS ${COMPILED_FILE_PARENT})
        file(MAKE_DIRECTORY ${COMPILED_FILE_PARENT})
    endif ()

    # Set the path of the compiled file.
    set(COMPILED_FILE "${COMPILED_FILE_PARENT}/${SOURCE_FILE_NAME}.spv")

    # Initialize the command to compile the shader
    set(COMPILE_COMMAND
            ${Vulkan_SLANG_EXECUTABLE}
            "${SOURCE_FILE}"
            -profile glsl_460
            -target spirv)

    # If the shader file has multiple entry points, add them to the command.
    if (SHADER_ENTRIES)
        foreach (E IN LISTS SHADER_ENTRIES)
            list(APPEND COMPILE_COMMAND -entry "${E}")
        endforeach ()
    endif ()

    # Finally, add the output file to the command.
    list(APPEND COMPILE_COMMAND -o "${COMPILED_FILE}")
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        list(APPEND COMPILE_COMMAND -g)
    endif ()

    # Run the command.
    add_custom_command(OUTPUT ${COMPILED_FILE}
            COMMAND ${COMPILE_COMMAND}
            DEPENDS ${SOURCE_FILE})

    # Set the shader as a dependency to Shader target. This ensures, the file is compiled before the target is
    # built.
    set_source_files_properties(${COMPILED_FILE} PROPERTIES GENERATED TRUE)
    target_sources(Shaders PRIVATE ${COMPILED_FILE})
endfunction()


addShader(${SHADERS_ROOT_DIR_SOURCE}/basic.slang "" "mainVS" "mainFS")
addShader(${SHADERS_ROOT_DIR_SOURCE}/environment.slang "" "mainVS" "mainFS")
addShader(${SHADERS_ROOT_DIR_SOURCE}/tonemapping.slang "")