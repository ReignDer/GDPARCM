add_requires("libflac", {configs = {runtimes = is_mode("debug") and "MTd" or "MT"}})
add_requires("libvorbis",{configs = {runtimes = is_mode("debug") and "MTd" or "MT"}})
add_requires("freetype", {configs = {harfbuzz = false, runtimes = is_mode("debug") and "MTd" or "MT"}})
add_requires("sheenbidi", {configs = {runtimes = is_mode("debug") and "MTd" or "MT"}})
add_requires("harfbuzz", {configs = {freetype = true, runtimes = is_mode("debug") and "MTd" or "MT"}})

target("SFML")
	set_kind("static")
	set_languages("c++20")
	set_runtimes("MT")
	add_defines("SFML_STATIC")

	add_packages("libflac", "libvorbis", "freetype","sheenbidi", "harfbuzz")

	set_targetdir("bin/".. outputdir .. "/SFML")
	set_objectdir("bin-int/".. outputdir .. "/SFML")
 	add_includedirs("src") 
	add_includedirs("include") 
	add_includedirs("extlibs") 
	add_includedirs("extlibs/headers/miniaudio") 
	add_includedirs("extlibs/headers/dr_mp3")
	add_includedirs("extlibs/headers/glad/include") 
	add_includedirs("extlibs/headers/qoi") 
	add_includedirs("extlibs/headers/stb_image") 
	add_includedirs("extlibs/headers/cpp-unicodelib") 
	add_includedirs("extlibs/headers/mingw") 
	add_includedirs("extlibs/headers/vulkan") 
	add_includedirs("extlibs/headers/wepoll") 

	add_headerfiles(
		"extlibs/**.h",
		"include/**.hpp",
		"src/SFML/PCH.hpp",
		"src/SFML/Audio/**.hpp",
		"src/SFML/Graphics/**.hpp",
		"src/SFML/System/EnumArray.hpp",
		"src/SFML/System/Utils.hpp",
		"src/SFML/Window/ClipboardImpl.hpp",
		"src/SFML/Window/CursorImpl.hpp",
		"src/SFML/Window/EGLCheck.hpp",
		"src/SFML/Window/EglContext.hpp",
		"src/SFML/Window/GlContext.hpp",
		"src/SFML/Window/InputImpl.hpp",
		"src/SFML/Window/JoystickImpl.hpp",
		"src/SFML/Window/JoystickManager.hpp",
		"src/SFML/Window/SensorManager.hpp",
		"src/SFML/Window/VideoMode.hpp",
		"src/SFML/Window/VulkanImpl.hpp",
		"src/SFML/Window/WindowImpl.hpp"
		
	)

	add_extrafiles(
		"include/**.inl"
	)

	add_files(
		"extlibs/**.c",
		"src/SFML/Audio/**.cpp",
		"src/SFML/Graphics/**.cpp",

		"src/SFML/System/Clock.cpp",
		"src/SFML/System/Err.cpp",
		"src/SFML/System/FileInputStream.cpp",
		"src/SFML/System/MemoryInputStream.cpp",
		"src/SFML/System/Sleep.cpp",
		"src/SFML/System/String.cpp",
		"src/SFML/System/TimeoutWithPredicate.cpp",
		"src/SFML/System/Utils.cpp",
		"src/SFML/System/Version.cpp",

		"src/SFML/Window/Clipboard.cpp",
		"src/SFML/Window/Context.cpp",
		"src/SFML/Window/Cursor.cpp",
		"src/SFML/Window/EGLCheck.cpp",
		"src/SFML/Window/EglContext.cpp",
		"src/SFML/Window/GlContext.cpp",
		"src/SFML/Window/GlResource.cpp",
		"src/SFML/Window/Joystick.cpp",
		"src/SFML/Window/JoystickManager.cpp",
		"src/SFML/Window/Keyboard.cpp",
		"src/SFML/Window/Mouse.cpp",
		"src/SFML/Window/Sensor.cpp",
		"src/SFML/Window/SensorManager.cpp",
		"src/SFML/Window/Touch.cpp",
		"src/SFML/Window/VideoMode.cpp",
		"src/SFML/Window/Vulkan.cpp",
		"src/SFML/Window/Window.cpp",
		"src/SFML/Window/WindowBase.cpp",
		"src/SFML/Window/WindowImpl.cpp"

	)

	if is_os("windows") then
		add_defines("WINVER=0x0A00")
		add_defines("_WIN32_WINNT=0x0A00") 
		add_defines("SFML_IS_BIG_ENDIAN=0")
		add_cxxflags("/utf-8", {tools = "msvc", force = true})
		add_headerfiles(
			"src/SFML/System/Win32/**.hpp",
			"src/SFML/Window/Win32/**.hpp"
		)

		add_files(
			"src/SFML/Main/MainWin32.cpp",
			"src/SFML/Network/Win32/**.cpp",
			"src/SFML/System/Win32/**.cpp",
			"src/SFML/Window/Win32/**.cpp"
		)
	end

	if is_mode("debug") then
		set_runtimes("MTd")
		set_symbols("debug")
	end

	if is_os("windows") and is_mode("asan") then
		set_runtimes("MTd")
		set_symbols("debug")
	end
	
	if is_mode("release") then
		set_runtimes("MT")
		set_optimize("fastest")
	end