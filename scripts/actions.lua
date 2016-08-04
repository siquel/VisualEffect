local embedded_shaders = {
  "cube"
}

local shader_root = path.join(VISEF_DIR, "shaders")
local out_dir = path.join(VISEF_DIR, "src", "shaders")
SHADERC = path.join(VISEF_DIR, "tools", "bin", "shaderc")

newaction {
  trigger = "embed-shader",
  description = "Build embedded shaders",
  execute = function()
    local vs_flags = "--type vertex -p vs_4_0 -O 3"
    local fs_flags = "--type fragment -p ps_4_0 -O 3"

    for i, directory in ipairs(embedded_shaders) do
      local popen = io.popen
      local dir = path.join(shader_root, directory)
      local pfile = popen("dir \"" .. dir .. "\" /b")
      for filename in pfile:lines() do
        local file_in = path.join(dir, filename)
        local file_out = string.gsub(filename, "(.sc)", ".bin.h")
        print (file_out)
        if (string.find(filename, "vs")) then
          os.execute (
            string.format("%s %s --platform windows -f \"%s\" -o \"%s\"",
            SHADERC,
            vs_flags,
            file_in,
            path.join(out_dir, file_out)
            )
          )
        elseif string.find(filename, "fs") then
          os.execute (
            string.format("%s %s --platform windows -f \"%s\" -o \"%s\"",
            SHADERC,
            fs_flags,
            file_in,
            path.join(out_dir, file_out)
            )
          )
        end
      end
      pfile:close()
    end

    os.exit(0)
  end
}
