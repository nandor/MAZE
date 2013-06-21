# This file is part of the MAZE project.
# Licensing information can be found in the LICENSE file
# (C) 2012 The MAZE project. All rights reserved.

cmake_minimum_required(VERSION 2.8)

include_directories("${PROJECT_SOURCE_DIR}/maze/vorbis")

set(SOURCES analysis.c
			bitrate.c
			block.c
			codebook.c
			envelope.c
			floor0.c
			floor1.c
			info.c
			lookup.c
			lpc.c
			lsp.c
			mapping0.c
			mdct.c
			psy.c
			registry.c
			res0.c
			sharedbook.c
			smallft.c
			synthesis.c
			vorbisenc.c
			vorbisfile.c
			window.c)

set(HEADERS backends.h
			bitrate.h
			codebook.h
			codec.h
			codec_internal.h
			envelope.h
			modes/floor_all.h
			books/floor/floor_books.h
			highlevel.h
			lookup.h
			lookup_data.h
			lpc.h
			lsp.h
			masking.h
			mdct.h
			misc.h
			os.h
			psy.h
			modes/psych_11.h
			modes/psych_16.h
			modes/psych_44.h
			modes/psych_8.h
			registry.h
			modes/residue_16.h
			modes/residue_44.h
			modes/residue_44u.h
			modes/residue_8.h
			scales.h
			modes/setup_11.h
			modes/setup_16.h
			modes/setup_22.h
			modes/setup_32.h
			modes/setup_44.h
			modes/setup_44u.h
			modes/setup_8.h
			modes/setup_X.h
			smallft.h
			vorbisenc.h
			vorbisfile.h
			window.h)

add_library(vorbis STATIC ${SOURCES} ${HEADERS})
target_link_libraries(vorbis ogg)
