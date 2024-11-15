workspace "mopi_Workspace"
    configurations { "Debug", "Release" }
    architecture "x86_64"

project "mopi"
    kind "SharedLib"
    language "C"
    cdialect "c99"

    targetdir("bin/")
    objdir("bin/obj")

    files {
        "include/**.h",
        "src/**.c",
        "src/**.m"
    }

    includedirs {
        "include/"
    }

    defines {
        "MOPI_EXPORT"
    }

    filter "system:macosx"
        links {"Cocoa.framework", "IOKit.framework", "CoreFoundation.framework"}

    filter "configurations:Debug"
        defines "MOPI_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "MOPI_RELEASE"
        optimize "On"

include "test"
