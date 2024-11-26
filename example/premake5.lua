workspace "mopi_example"
    configurations { "Debug", "Release" }
    architecture "x86_64"

include "../"

Includes={}
Includes["mopi"] = "../include"

project "mopi_example"
    kind "ConsoleApp"
    language "c"
    cdialect "c99"

    targetdir("../bin/")
    objdir("../bin/obj/")

    files {
        "main.c"
    }

    includedirs {
        "%{Includes.mopi}"
    }

    links {
        "mopi"
    }

    filter "configurations:Debug"
        defines "DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "RELEASE"
        optimize "On"