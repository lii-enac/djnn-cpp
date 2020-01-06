
namespace djnn {
	void init_audio ()
	{
		#ifdef USE_OPENAL
		void init_audio_openal ();
		init_audio_openal();
		#endif
	}

	void clear_audio ()
	{
		#ifdef USE_OPENAL
		void clear_audio_openal ();
		clear_audio_openal();
		#endif
	}

}
