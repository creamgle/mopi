project "mopi"
    kind "SharedLib"
    language "c"
    cdialect "c99"

    targetdir("bin/")
    objdir("bin/obj/")

    files {
        "include/**.h",
        "src/**.h",
        "src/**.c"
    }

    includedirs {
        "include"
    }

    filter "configurations:Debug"
        defines "MOPI_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "MOPI_RELEASE"
        optimize "On"
