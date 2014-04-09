
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

MESSAGE (STATUS "configuring video_engine !!!")

include_directories(../ modules/remote_bitrate_estimator ./include ./)

set (VIE_SRC 
	vie_network_impl.cc
	veruse_frame_detector.cc
	vie_encoder.cc
	vie_input_manager.cc
	call_stats_unittest.cc
	vie_sender.cc
	stream_synchronization.cc
	vie_render_manager.cc
	vie_channel_group.cc
	vie_sync_module.cc
	vie_external_codec_impl.cc
	overuse_frame_detector_unittest.cc
	vie_capture_impl.cc
	vie_base_impl.cc
	vie_frame_provider_base.cc
	stream_synchronization_unittest.cc
	vie_capturer.cc
	vie_rtp_rtcp_impl.cc
	vie_impl.cc
	vie_channel_manager.cc
	encoder_state_feedback_unittest.cc
	vie_remb.cc
	call_stats.cc
	vie_codec_unittest.cc
	vie_ref_count.cc
	encoder_state_feedback.cc
	vie_codec_impl.cc
	vie_remb_unittest.cc
	vie_channel.cc
	vie_render_impl.cc
	vie_manager_base.cc
	vie_image_process_impl.cc
	vie_receiver.cc
	vie_shared_data.cc
	vie_file_image.cc
	vie_renderer.cc
)







add_library (vie ${VIE_SRC})


target_link_libraries (vie) 