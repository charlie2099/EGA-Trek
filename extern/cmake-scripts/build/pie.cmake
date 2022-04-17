#------------------------------------------------------------------------------
# Sets up the build global build options
#------------------------------------------------------------------------------
if (CMAKE_VERSION VERSION_GREATER_EQUAL 3.14.0)
    include(CheckPIESupported)
    check_pie_supported()

    if(NOT CMAKE_CXX_LINK_PIE_SUPPORTED)
        message(STATUS "POSITION INDEPENDENT CODE: \t OFF")
    else()
        message(STATUS "POSITION INDEPENDENT CODE: \t\t ON")
    endif()
endif()




