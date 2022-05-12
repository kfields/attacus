# bgfx.cmake - bgfx building in cmake
# Written in 2017 by Joshua Brookover <joshua.al.brookover@gmail.com>

# To the extent possible under law, the author(s) have dedicated all copyright
# and related and neighboring rights to this software to the public domain
# worldwide. This software is distributed without any warranty.

# You should have received a copy of the CC0 Public Domain Dedication along with
# this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

# shaderc_parse(
#	FILE filename
#	OUTPUT filename
#	FRAGMENT|VERTEX|COMPUTE
#	ANDROID|ASM_JS|IOS|LINUX|NACL|OSX|WINDOWS
#	[PROFILE profile]
#	[O 0|1|2|3]
#	[VARYINGDEF filename]
#	[BIN2C filename]
#	[INCLUDES include;include]
#	[DEFINES include;include]
#	[DEPENDS]
#	[PREPROCESS]
#	[RAW]
#	[VERBOSE]
#	[DEBUG]
#	[DISASM]
#	[WERROR]
# )
function( shaderc_parse ARG_OUT )
    #message(shaderc_parse: ${ARG_OUT})
	cmake_parse_arguments( ARG "DEPENDS;ANDROID;ASM_JS;IOS;LINUX;NACL;OSX;WINDOWS;PREPROCESS;RAW;FRAGMENT;VERTEX;COMPUTE;VERBOSE;DEBUG;DISASM;WERROR" "FILE;OUTPUT;VARYINGDEF;BIN2C;PROFILE;O" "INCLUDES;DEFINES" ${ARGN} )
	set( CLI "" )

	# -f
	if( ARG_FILE )
		list( APPEND CLI "-f" "${ARG_FILE}" )
	endif()

	# -i
	if( ARG_INCLUDES )
		foreach( INCLUDE ${ARG_INCLUDES} )
			list( APPEND CLI "-i" )
			list( APPEND CLI "${INCLUDE}" )			
		endforeach()
	endif()

	# -o
	if( ARG_OUTPUT )
		list( APPEND CLI "-o" "${ARG_OUTPUT}" )
	endif()

	# --bin2c
	if( ARG_BIN2C )
		list( APPEND CLI "--bin2c" "${ARG_BIN2C}" )
	endif()

	# --depends
	if( ARG_DEPENDS )
		list( APPEND CLI "--depends" )
	endif()

	# --platform
	set( PLATFORM "" )
	set( PLATFORMS "ANDROID;ASM_JS;IOS;LINUX;NACL;OSX;WINDOWS" )
	foreach( P ${PLATFORMS} )
		if( ARG_${P} )
			if( PLATFORM )
				message( SEND_ERROR "Call to shaderc_parse() cannot have both flags ${PLATFORM} and ${P}." )
				return()
			endif()
			set( PLATFORM "${P}" )
		endif()
	endforeach()
	if( "${PLATFORM}" STREQUAL "" )
		message( SEND_ERROR "Call to shaderc_parse() must have a platform flag: ${PLATFORMS}" )
		return()
	elseif( "${PLATFORM}" STREQUAL "ANDROID" )
		list( APPEND CLI "--platform" "android" )
	elseif( "${PLATFORM}" STREQUAL "ASM_JS" )
		list( APPEND CLI "--platform" "asm.js" )
	elseif( "${PLATFORM}" STREQUAL "IOS" )
		list( APPEND CLI "--platform" "ios" )
	elseif( "${PLATFORM}" STREQUAL "LINUX" )
		list( APPEND CLI "--platform" "linux" )
	elseif( "${PLATFORM}" STREQUAL "NACL" )
		list( APPEND CLI "--platform" "nacl" )
	elseif( "${PLATFORM}" STREQUAL "OSX" )
		list( APPEND CLI "--platform" "osx" )
	elseif( "${PLATFORM}" STREQUAL "WINDOWS" )
		list( APPEND CLI "--platform" "windows" )
	endif()

	# --preprocess
	if( ARG_PREPROCESS )
		list( APPEND CLI "--preprocess" )
	endif()

	# --define
	if( ARG_DEFINES )
		list( APPEND CLI "--defines" )
		set( DEFINES "" )
		foreach( DEFINE ${ARG_DEFINES} )
			if( NOT "${DEFINES}" STREQUAL "" )
				set( DEFINES "${DEFINES}\\\\;${DEFINE}" )
			else()
				set( DEFINES "${DEFINE}" )
			endif()
		endforeach()
		list( APPEND CLI "${DEFINES}" )
	endif()

	# --raw
	if( ARG_RAW )
		list( APPEND CLI "--raw" )
	endif()

	# --type
	set( TYPE "" )
	set( TYPES "FRAGMENT;VERTEX;COMPUTE" )
	foreach( T ${TYPES} )
		if( ARG_${T} )
			if( TYPE )
				message( SEND_ERROR "Call to shaderc_parse() cannot have both flags ${TYPE} and ${T}." )
				return()
			endif()
			set( TYPE "${T}" )
		endif()
	endforeach()
	if( "${TYPE}" STREQUAL "" )
		message( SEND_ERROR "Call to shaderc_parse() must have a type flag: ${TYPES}" )
		return()
	elseif( "${TYPE}" STREQUAL "FRAGMENT" )
		list( APPEND CLI "--type" "fragment" )
	elseif( "${TYPE}" STREQUAL "VERTEX" )
		list( APPEND CLI "--type" "vertex" )
	elseif( "${TYPE}" STREQUAL "COMPUTE" )
		list( APPEND CLI "--type" "compute" )
	endif()

	# --varyingdef
	if( ARG_VARYINGDEF )
		list( APPEND CLI "--varyingdef" "${ARG_VARYINGDEF}" )
	endif()

	# --verbose
	if( ARG_VERBOSE )
		list( APPEND CLI "--verbose" )
	endif()

	# --debug
	if( ARG_DEBUG )
		list( APPEND CLI "--debug" )
	endif()

	# --disasm
	if( ARG_DISASM )
		list( APPEND CLI "--disasm" )
	endif()

	# --profile
	if( ARG_PROFILE )
		list( APPEND CLI "--profile" "${ARG_PROFILE}" )
	endif()

	# -O
	if( ARG_O )
		list( APPEND CLI "-O" "${ARG_O}" )
	endif()

	# --Werror
	if( ARG_WERROR )
		list( APPEND CLI "--Werror" )
	endif()

	set( ${ARG_OUT} ${CLI} PARENT_SCOPE )
endfunction()

function( add_bgfx_shader THIS FILE)
    #message(add_bgfx_shader: ${FILE})
	get_filename_component( FILENAME "${FILE}" NAME_WE )
	string( SUBSTRING "${FILENAME}" 0 2 TYPE )
	if( "${TYPE}" STREQUAL "fs" )
		set( TYPE "FRAGMENT" )
		set( D3D_PREFIX "ps" )
	elseif( "${TYPE}" STREQUAL "vs" )
		set( TYPE "VERTEX" )
		set( D3D_PREFIX "vs" )
	elseif( "${TYPE}" STREQUAL "cs" )
		set( TYPE "COMPUTE" )
 		set( D3D_PREFIX "cs" )
	else()
		set( TYPE "" )
	endif()

	if( NOT "${TYPE}" STREQUAL "" )
		set( COMMON FILE ${FILE} ${TYPE} INCLUDES ${BGFX_ROOT}/src )
		set( OUTPUTS "" )
		set( OUTPUTS_PRETTY "" )

		if( WIN32 )
			# dx9
			if( NOT "${TYPE}" STREQUAL "COMPUTE" )
				set( DX9_OUTPUT ${ATT_ROOT}/examples/runtime/shaders/dx9/${FILENAME}.bin )
				shaderc_parse( DX9 ${COMMON} WINDOWS PROFILE ${D3D_PREFIX}_3_0 O 3 OUTPUT ${DX9_OUTPUT} )
				list( APPEND OUTPUTS "DX9" )
				set( OUTPUTS_PRETTY "${OUTPUTS_PRETTY}DX9, " )
			endif()

			# dx11
			set( DX11_OUTPUT ${ATT_ROOT}/examples/runtime/shaders/dx11/${FILENAME}.bin )
			if( NOT "${TYPE}" STREQUAL "COMPUTE" )
				shaderc_parse( DX11 ${COMMON} WINDOWS PROFILE ${D3D_PREFIX}_5_0 O 3 OUTPUT ${DX11_OUTPUT} )
			else()
				shaderc_parse( DX11 ${COMMON} WINDOWS PROFILE ${D3D_PREFIX}_5_0 O 1 OUTPUT ${DX11_OUTPUT} )
			endif()
			list( APPEND OUTPUTS "DX11" )
			set( OUTPUTS_PRETTY "${OUTPUTS_PRETTY}DX11, " )
		endif()

		if( APPLE )
			# metal
			set( METAL_OUTPUT ${ATT_ROOT}/examples/runtime/shaders/metal/${FILENAME}.bin )
			shaderc_parse( METAL ${COMMON} OSX PROFILE metal OUTPUT ${METAL_OUTPUT} )
			list( APPEND OUTPUTS "METAL" )
			set( OUTPUTS_PRETTY "${OUTPUTS_PRETTY}Metal, " )
		endif()

		# essl
		if( NOT "${TYPE}" STREQUAL "COMPUTE" )
			set( ESSL_OUTPUT ${ATT_ROOT}/examples/runtime/shaders/essl/${FILENAME}.bin )
			shaderc_parse( ESSL ${COMMON} ANDROID OUTPUT ${ESSL_OUTPUT} )
			list( APPEND OUTPUTS "ESSL" )
			set( OUTPUTS_PRETTY "${OUTPUTS_PRETTY}ESSL, " )
		endif()

		# glsl
		set( GLSL_OUTPUT ${ATT_ROOT}/examples/runtime/shaders/glsl/${FILENAME}.bin )
		if( NOT "${TYPE}" STREQUAL "COMPUTE" )
			shaderc_parse( GLSL ${COMMON} LINUX PROFILE 120 OUTPUT ${GLSL_OUTPUT} )
		else()
			shaderc_parse( GLSL ${COMMON} LINUX PROFILE 430 OUTPUT ${GLSL_OUTPUT} )
		endif()
		list( APPEND OUTPUTS "GLSL" )
		set( OUTPUTS_PRETTY "${OUTPUTS_PRETTY}GLSL, " )

		# spirv
		if( NOT "${TYPE}" STREQUAL "COMPUTE" )
			set( SPIRV_OUTPUT ${ATT_ROOT}/examples/runtime/shaders/spirv/${FILENAME}.bin )
			shaderc_parse( SPIRV ${COMMON} LINUX PROFILE spirv OUTPUT ${SPIRV_OUTPUT} )
			list( APPEND OUTPUTS "SPIRV" )
			set( OUTPUTS_PRETTY "${OUTPUTS_PRETTY}SPIRV" )
			set( OUTPUT_FILES "" )
			set( COMMANDS "" )
		endif()

		foreach( OUT ${OUTPUTS} )
            #message(add_bgfx_shader:OUTPUTS: ${OUT})
			list( APPEND OUTPUT_FILES ${${OUT}_OUTPUT} )
			#list( APPEND COMMANDS COMMAND "$<TARGET_FILE:shaderc>" ${${OUT}} )
            list( APPEND COMMANDS COMMAND shadercRelease ${${OUT}} )
			get_filename_component( OUT_DIR ${${OUT}_OUTPUT} DIRECTORY )
			file( MAKE_DIRECTORY ${OUT_DIR} )
		endforeach()

		file( RELATIVE_PATH PRINT_NAME ${ATT_ROOT}/examples ${FILE} )
        #message(add_bgfx_shader:add_custom_command: ${FILE}:${OUTPUT_FILES}:${COMMANDS})
		add_custom_command(
            TARGET ${THIS}
            POST_BUILD
			MAIN_DEPENDENCY
			${FILE}
			#OUTPUT
			#${OUTPUT_FILES}
			${COMMANDS}
			COMMENT "Compiling shader ${PRINT_NAME} for ${OUTPUTS_PRETTY}"
		)
	endif()
endfunction()
