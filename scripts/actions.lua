local embedded_shaders = {
  "cube",
  "postprocess"
}

local shader_root = path.join(VISEF_DIR, "shaders")
local out_dir = path.join(VISEF_DIR, "src", "shaders")
SHADERC = path.join(VISEF_DIR, "tools", "bin", "shaderc")

function basename(str)
	local name = string.gsub(str, "(.*/)(.*)", "%2")
	return name
end

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
        if (string.find(filename, "vs")) then
          os.execute (
            string.format("%s %s --platform windows -f \"%s\" -o \"%s\" --bin2c %s",
            SHADERC,
            vs_flags,
            file_in,
            path.join(out_dir, file_out),
            string.gsub(filename, ".sc", "") .. "_dx11"
            )
          )
        elseif string.find(filename, "fs") then
          os.execute (
            string.format("%s %s --platform windows -f \"%s\" -o \"%s\" --bin2c %s",
            SHADERC,
            fs_flags,
            file_in,
            path.join(out_dir, file_out),
            string.gsub(filename, ".sc", "") .. "_dx11"
            )
          )
        end
      end
      pfile:close()
    end

    os.exit(0)
  end
}
