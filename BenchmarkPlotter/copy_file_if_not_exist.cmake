cmake_minimum_required(VERSION 3.24)

# Access the properties set in CMakeLists.txt
set(source_file $ENV{SOURCE_FILE})
set(destination_dir $ENV{DEST_DIR})
get_filename_component(source_file_name "${source_file}" NAME)
set(destination_file "${destination_dir}/${source_file_name}")

message("Checking if ${destination_file} exists.")

# Check if the destination file already exists
if (NOT EXISTS "${destination_file}")
    message("Copying ${source_file} to ${destination_dir}")
    if (EXISTS "${source_file}")
        file(COPY "${source_file}" DESTINATION "${destination_dir}")
    else ()
        message("imgui.ini yet not exist")
    endif ()
else ()
    message("File already exists: ${destination_file}")
endif ()
