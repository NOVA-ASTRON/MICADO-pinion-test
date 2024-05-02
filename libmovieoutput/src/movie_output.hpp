/*
 * movie_output.hpp
 * 
 * Copyright 2019 janrinze <janrinze@debian9work>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#ifndef MOVIE_OUTPUT_HPP
#define MOVIE_OUTPUT_HPP

extern "C" {
	#include <libavcodec/avcodec.h>
	#include <libavutil/imgutils.h>
	#include <libavutil/opt.h>
	#include <libswscale/swscale.h>
}
class movie_output
{
	public:
		movie_output(const char * name, float fps ,int w,int h);
		void ffmpeg_encoder_encode_frame(uint8_t *rgb);
		void ffmpeg_encoder_finish(void);
			
	private:
		void ffmpeg_encoder_start(const char *filename, int codec_id, float fps, int width, int height);
		void ffmpeg_encoder_set_frame_yuv_from_rgb(uint8_t *rgb);
		AVCodecContext *c;
		AVFrame *frame;
		AVPacket pkt;
		FILE *file;
		struct SwsContext *sws_context;
		int frame_counter;
};

#endif /* MOVIE_OUTPUT_HPP */ 
