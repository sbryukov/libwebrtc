
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)


include_directories(../ modules/remote_bitrate_estimator ./include ./)

set (VOE_SRC 
	#oe_codec_unittest.cc
	#output_mixer_unittest.cc
	voe_file_impl.cc
	dtmf_inband_queue.cc
	channel_manager.cc
	voe_codec_impl.cc
	channel.cc
	#voe_neteq_stats_unittest.cc
	voe_hardware_impl.cc
	#voe_audio_processing_unittest.cc
	voe_dtmf_impl.cc
	#voe_base_unittest.cc
	voe_volume_control_impl.cc
	level_indicator.cc
	output_mixer.cc
	voe_network_impl.cc
	voe_audio_processing_impl.cc
	voice_engine_impl.cc
	voe_external_media_impl.cc
	statistics.cc
	transmit_mixer.cc
	voe_call_report_impl.cc
	voe_neteq_stats_impl.cc
	shared_data.cc
	voe_rtp_rtcp_impl.cc
	monitor_module.cc
	utility.cc
	voe_base_impl.cc
	voe_video_sync_impl.cc
	output_mixer_internal.cc
	#channel_unittest.cc
	#transmit_mixer_unittest.cc
	dtmf_inband.cc
)

add_library (voe ${VOE_SRC})
target_link_libraries (voe) 