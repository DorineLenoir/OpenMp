# Download video and font files into dat/
set(VIDEO_WEBM "Spring_-_Blender_Open_Movie.webm")
set(VIDEO_YUV "spring_2048x858_168f.yuv")
set(VIDEO_URL "https://upload.wikimedia.org/wikipedia/commons/a/a5/${VIDEO_WEBM}")

set(TTF_NAME "DejaVuSans.ttf")
set(TTF_URL "https://preesm.github.io/assets/downloads/${TTF_NAME}")

# FFMPEG download parameters
if(WIN32)
	set(FFMPEG_FOLDER "ffmpeg-master-latest-win64-gpl")
	set(FFMPEG_ARCHIVE "${FFMPEG_FOLDER}.zip")
	set(FFMPEG_URL "https://github.com/BtbN/FFmpeg-Builds/releases/download/latest/${FFMPEG_ARCHIVE}")
	set(FFMPEG_EXTRACT "${FFMPEG_FOLDER}/bin/ffmpeg.exe")
elseif(UNIX AND NOT APPLE)
	set(FFMPEG_FOLDER "ffmpeg-master-latest-linux64-gpl")
	set(FFMPEG_ARCHIVE "${FFMPEG_FOLDER}.tar.xz")
	set(FFMPEG_URL "https://github.com/BtbN/FFmpeg-Builds/releases/download/latest/${FFMPEG_ARCHIVE}")
	set(FFMPEG_EXTRACT "${FFMPEG_FOLDER}/bin/ffmpeg")
elseif(APPLE)
	set(FFMPEG_FOLDER "")	#EMPTY
	set(FFMPEG_ARCHIVE "ffmpeg.zip")
	set(FFMPEG_URL "https://evermeet.cx/ffmpeg/getrelease/zip")
	set(FFMPEG_EXTRACT "ffmpeg")
endif()


MESSAGE("Download dat content.")

# MP4 into FFMPEG
if (NOT EXISTS ${DAT_DIR}/${VIDEO_YUV})
	if (NOT EXISTS ${DAT_DIR}/${VIDEO_WEBM})
		# Download video
		MESSAGE("Downloading ${VIDEO_WEBM}")
		file(DOWNLOAD "${VIDEO_URL}" "${DAT_DIR}/${VIDEO_WEBM}"
					SHOW_PROGRESS)
	endif ()

	MESSAGE("Downloading ${FFMPEG_ARCHIVE}")
	if (NOT EXISTS ${DAT_DIR}/${FFMPEG_ARCHIVE})
		file(DOWNLOAD "${FFMPEG_URL}" "${DAT_DIR}/${FFMPEG_ARCHIVE}"
					SHOW_PROGRESS)
	endif()

	MESSAGE("Extracting ${FFMPEG_ARCHIVE}")
	file(ARCHIVE_EXTRACT
				INPUT "${DAT_DIR}/${FFMPEG_ARCHIVE}"
				DESTINATION ${DAT_DIR}/
				PATTERNS ${FFMPEG_EXTRACT}
				VERBOSE)

	MESSAGE("Converting ${VIDEO_WEBM} into ${VIDEO_YUV}")
	MESSAGE("Running command: ${DAT_DIR}/${FFMPEG_EXTRACT} -i ${DAT_DIR}/${VIDEO_WEBM} -ss 00:00:50.00 -t 7.0 -vf scale=2048:858 -c:v rawvideo -pix_fmt yuv420p ${DAT_DIR}/${VIDEO_YUV}")
	execute_process(COMMAND ${DAT_DIR}/${FFMPEG_EXTRACT} -i ${DAT_DIR}/${VIDEO_WEBM} -ss 00:00:50.00 -t 7.0 -vf scale=2048:858 -c:v rawvideo -pix_fmt yuv420p ${DAT_DIR}/${VIDEO_YUV})

	file(REMOVE ${DAT_DIR}/${FFMPEG_ARCHIVE})

	if(NOT APPLE)
		file(REMOVE_RECURSE ${DAT_DIR}/${FFMPEG_FOLDER})
	else()
		file(REMOVE "${DAT_DIR}/${FFMPEG_ARCHIVE}")
	endif()

	file(REMOVE ${DAT_DIR}/${VIDEO_WEBM})
endif()


# Downloading font file
if(NOT EXISTS ${DAT_DIR}/${TTF_NAME})
	MESSAGE("Downloading ${TTF_NAME}")
	file(DOWNLOAD ${TTF_URL} "${DAT_DIR}/${TTF_NAME}"
			EXPECTED_MD5 BE189A7E2711CDF2A7F6275C60CBC7E2)
endif()

