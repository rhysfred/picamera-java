/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (C) 2020, Raspberry Pi (Trading) Ltd.
 *
 * rpicam_vid.cpp - libcamera video record app.
 */

#include <chrono>
#include <poll.h>
#include <signal.h>
#include <sys/signalfd.h>
#include <sys/stat.h>

#include "core/rpicam_encoder.hpp"
#include "output/output.hpp"
#include "rpicam_java.hpp"

using namespace std::placeholders;

// Some keypress/signal handling.

static int signal_received;
enum State
{
	DISABLED = 0,
	WAITING_KEYFRAME = 1,
	RUNNING = 2
};
State state_ = WAITING_KEYFRAME;

enum Flag
{
	FLAG_NONE = 0,
	FLAG_KEYFRAME = 1,
	FLAG_RESTART = 2
};

std::atomic<bool> enable_;
int64_t time_offset_ = 0;
int64_t last_timestamp_ = 0;

static void default_signal_handler(int signal_number)
{
	signal_received = signal_number;
	LOG(1, "Received signal " << signal_number);
	
}

static int get_colourspace_flags(std::string const &codec)
{
	if (codec == "mjpeg" || codec == "yuv420")
		return RPiCamEncoder::FLAG_VIDEO_JPEG_COLOURSPACE;
	else
		return RPiCamEncoder::FLAG_VIDEO_NONE;
}

// The main event loop for the application.

static void event_loop(RPiCamEncoder &app, boolean (*f)(void *, size_t))
{
	VideoOptions const *options = app.GetOptions();
	//std::unique_ptr<Output> output = std::unique_ptr<Output>(Output::Create(options));
	std::unique_ptr<Output> output = std::unique_ptr<Output>(JavaOutput::JavaOutput(options, f));
	app.SetEncodeOutputReadyCallback(std::bind(&Output::OutputReady, output.get(), _1, _2, _3, _4));
	//app.SetMetadataReadyCallback(std::bind(&Output::MetadataReady, output.get(), _1));

	app.OpenCamera();
	app.ConfigureVideo(get_colourspace_flags(options->codec));
	app.StartEncoder();
	app.StartCamera();

	signal(SIGPIPE, default_signal_handler);

	for (unsigned int count = 0; ; count++)
	{
		RPiCamEncoder::Msg msg = app.Wait();
		if (msg.type == RPiCamApp::MsgType::Timeout)
		{
			LOG_ERROR("Restarting rpicam device");
			app.StopCamera();
			app.StartCamera();
			continue;
		}
		if (msg.type == RPiCamEncoder::MsgType::Quit)
			return;
		else if (msg.type != RPiCamEncoder::MsgType::RequestComplete)
			throw std::runtime_error("unrecognised message!");

		LOG(2, "Viewfinder frame " << count);
		if (signal_received == SIGPIPE)
		{
			app.StopCamera();
			app.StopEncoder();
			return;
		}

		CompletedRequestPtr &completed_request = std::get<CompletedRequestPtr>(msg.payload);
		app.EncodeBuffer(completed_request, app.VideoStream());
		app.ShowPreview(completed_request, app.VideoStream());
	}
}

void start_stream(boolean (*f)(void *, size_t))
{

	// dummy, just to get compiling for the moment
	int argc = 1;
	char *argv[1];
	// end dummy
	try
	{
		RPiCamEncoder app;
		VideoOptions *options = app.GetOptions();
		if (options->Parse(argc, argv))
		{
			if (options->verbose >= 2)
				options->Print();

			event_loop(app, f);
		}
	}
	catch (std::exception const &e)
	{
		LOG_ERROR("ERROR: *** " << e.what() << " ***");
		return;
	}
	return;
}