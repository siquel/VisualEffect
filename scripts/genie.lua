VISEF_DIR = (path.getabsolute("..") .. "/")
local VISEF_THIRD_DIR = (VISEF_DIR .. "3rdparty/")
local VISEF_BUILD_DIR = (VISEF_DIR .. ".build/")
BGFX_DIR = (VISEF_THIRD_DIR .. "bgfx/")
BX_DIR = (VISEF_THIRD_DIR .. "bx/")

solution "visef"
  configurations {
    "debug",
    "release"
  }

  platforms {
    "x32",
    "x64"
  }

  language "C++"
  startproject "demo"

  configuration {} -- reset configuration

function copyLib() end
function strip() end

configuration { "vs*" }
  	includedirs {
  		path.join(BX_DIR, "include/compat/msvc")
  	}
  	links { "psapi"}

  	linkoptions {
  		"/ignore:4199", -- LNK4199: /DELAYLOAD:*.dll ignored; no imports found from *.dll
  		"/ignore:4221", -- LNK4221: This object file does not define any previously undefined public symbols, so it will not be used by any link operation that consumes this library
  		"/ignore:4099", -- LNK4099: The linker was unable to find your .pdb file.
  	}

  	defines {
  		"__STDC_CONSTANT_MACROS",
  		"__STDC_FORMAT_MACROS",
  		"__STDC_LIMIT_MACROS",
  		"BX_CONFIG_ENABLE_MSVC_LEVEL4_WARNINGS=1"
  	}
    configuration {} -- reset configuration

dofile(path.join(BX_DIR, "scripts", "toolchain.lua"))
dofile(path.join(BGFX_DIR, "scripts", "bgfx.lua"))
toolchain(VISEF_BUILD_DIR, VISEF_THIRD_DIR)

project "demo"
  kind "ConsoleApp"

  includedirs {
    path.join(BX_DIR, "include"),
    path.join(BGFX_DIR, "include"),
    path.join(VISEF_THIRD_DIR),
    path.join(VISEF_DIR, "src")
  }

  links {
    "bgfx"
  }

  defines {
    "GLM_FORCE_RADIANS", -- enable radians
  }

  files {
    path.join(VISEF_DIR, "src", "**.cpp"),
    path.join(VISEF_DIR, "src", "**.h"),
  }

group "libs"

bgfxProject("", "StaticLib", { "BGFX_CONFIG_RENDERER_OPENGL=44" })

group "tools"
dofile("shaderc.lua")
