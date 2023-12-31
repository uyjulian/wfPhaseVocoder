//---------------------------------------------------------------------------
/*
	TVP2 ( T Visual Presenter 2 )  A script authoring tool
	Copyright (C) 2000 W.Dee <dee@kikyou.info> and contributors

	See details of license at "license.txt"
*/
//---------------------------------------------------------------------------
// Wave Player interface
//---------------------------------------------------------------------------
#ifndef WaveIntfH
#define WaveIntfH

#if 0
#include "tjsNative.h"
#include "SoundBufferBaseIntf.h"
#include "tjsUtils.h"
#endif

/*[*/
//---------------------------------------------------------------------------
// Sound Global Focus Mode
//---------------------------------------------------------------------------
#if 0
enum tTVPSoundGlobalFocusMode
{
	/*0*/ sgfmNeverMute,			// never mutes
	/*1*/ sgfmMuteOnMinimize,		// will mute on the application minimize
	/*2*/ sgfmMuteOnDeactivate		// will mute on the application deactivation
};
#endif
//---------------------------------------------------------------------------



/*]*/
//---------------------------------------------------------------------------
// GUID identifying WAVEFORMATEXTENSIBLE sub format
//---------------------------------------------------------------------------
#if 0
extern tjs_uint8 TVP_GUID_KSDATAFORMAT_SUBTYPE_PCM[16];
extern tjs_uint8 TVP_GUID_KSDATAFORMAT_SUBTYPE_IEEE_FLOAT[16];
#endif
//---------------------------------------------------------------------------


/*[*/
//---------------------------------------------------------------------------
// PCM data format (internal use)
//---------------------------------------------------------------------------
#if 0
struct tTVPWaveFormat
{
	tjs_uint SamplesPerSec; // sample granule per sec
	tjs_uint Channels;
	tjs_uint BitsPerSample; // per one sample
	tjs_uint BytesPerSample; // per one sample
	tjs_uint64 TotalSamples; // in sample granule; unknown for zero
	tjs_uint64 TotalTime; // in ms; unknown for zero
	tjs_uint32 SpeakerConfig; // bitwise OR of SPEAKER_* constants
	bool IsFloat; // true if the data is IEEE floating point
	bool Seekable;
};
#endif
//---------------------------------------------------------------------------



/*]*/
//---------------------------------------------------------------------------
// PCM bit depth converter
//---------------------------------------------------------------------------
#if 0
TJS_EXP_FUNC_DEF(void, TVPConvertPCMTo16bits, (tjs_int16 *output, const void *input, const tTVPWaveFormat &format, tjs_int count, bool downmix));
TJS_EXP_FUNC_DEF(void, TVPConvertPCMTo16bits, (tjs_int16 *output, const void *input, tjs_int channels, tjs_int bytespersample, tjs_int bitspersample, bool isfloat, tjs_int count, bool downmix));
TJS_EXP_FUNC_DEF(void, TVPConvertPCMToFloat, (float *output, const void *input, tjs_int channels, tjs_int bytespersample, tjs_int bitspersample, bool isfloat, tjs_int count));
TJS_EXP_FUNC_DEF(void, TVPConvertPCMToFloat, (float *output, const void *input, const tTVPWaveFormat &format, tjs_int count));
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
// tTVPWaveDecoder interface
//---------------------------------------------------------------------------
class tTVPWaveDecoder
{
public:
	virtual ~tTVPWaveDecoder() {};

	virtual void GetFormat(tTVPWaveFormat & format) = 0;
		/* Retrieve PCM format, etc. */

	virtual bool Render(void *buf, tjs_uint bufsamplelen, tjs_uint& rendered) = 0;
		/*
			Render PCM from current position.
			where "buf" is a destination buffer, "bufsamplelen" is the buffer's
			length in sample granule, "rendered" is to be an actual number of
			written sample granule.
			returns whether the decoding is to be continued.
			because "redered" can be lesser than "bufsamplelen", the player
			should not end until the returned value becomes false.
		*/

	virtual bool SetPosition(tjs_uint64 samplepos) = 0;
		/*
			Seek to "samplepos". "samplepos" must be given in unit of sample granule.
			returns whether the seeking is succeeded.
		*/
};
//---------------------------------------------------------------------------
class tTVPWaveDecoderCreator
{
public:
	virtual tTVPWaveDecoder * Create(const ttstr & storagename,
		const ttstr &extension) = 0;
		/*
			Create tTVPWaveDecoder instance. returns NULL if failed.
		*/
};
#endif
//---------------------------------------------------------------------------




//---------------------------------------------------------------------------
// tTVPWaveDecoder interface management
//---------------------------------------------------------------------------
#if 0
extern void TVPRegisterWaveDecoderCreator(tTVPWaveDecoderCreator *d);
extern void TVPUnregisterWaveDecoderCreator(tTVPWaveDecoderCreator *d);
extern tTVPWaveDecoder *  TVPCreateWaveDecoder(const ttstr & storagename);
#endif
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// interface for basic filter management
//---------------------------------------------------------------------------
class tTVPSampleAndLabelSource;
class iTVPBasicWaveFilter
{
public:
	// recreate filter. filter will remain owned by the each filter instance.
	virtual tTVPSampleAndLabelSource * Recreate(tTVPSampleAndLabelSource * source) = 0;
	virtual void Clear(void) = 0;
	virtual void Update(void) = 0;
	virtual void Reset(void) = 0;
};
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
// tTJSNI_BaseWaveSoundBuffer
//---------------------------------------------------------------------------
#if 0
class tTVPWaveLoopManager;
class tTJSNI_BaseWaveSoundBuffer : public tTJSNI_SoundBuffer
{
	typedef tTJSNI_SoundBuffer inherited;

	iTJSDispatch2 * WaveFlagsObject;
	iTJSDispatch2 * WaveLabelsObject;

	struct tFilterObjectAndInterface
	{
		tTJSVariant Filter; // filter object
		iTVPBasicWaveFilter * Interface; // filter interface
		tFilterObjectAndInterface(
			const tTJSVariant & filter,
			iTVPBasicWaveFilter * interf) :
			Filter(filter), Interface(interf) {;}
	};
	std::vector<tFilterObjectAndInterface> FilterInterfaces; // backupped filter interface array

protected:
	tTVPWaveLoopManager * LoopManager; // will be set by tTJSNI_WaveSoundBuffer
	tTVPSampleAndLabelSource * FilterOutput; // filter output
	iTJSDispatch2 * Filters; // wave filters array (TJS2 array object)
public:
	tTJSNI_BaseWaveSoundBuffer();
	tjs_error TJS_INTF_METHOD
	Construct(tjs_int numparams, tTJSVariant **param,
		iTJSDispatch2 *tjs_obj);
	void TJS_INTF_METHOD Invalidate();

protected:
	void InvokeLabelEvent(const ttstr & name);
	void RecreateWaveLabelsObject();
	void RebuildFilterChain();
	void ClearFilterChain();
	void ResetFilterChain();
	void UpdateFilterChain();

public:
	iTJSDispatch2 * GetWaveFlagsObjectNoAddRef();
	iTJSDispatch2 * GetWaveLabelsObjectNoAddRef();
	tTVPWaveLoopManager * GetWaveLoopManager() const { return LoopManager; }
	iTJSDispatch2 * GetFiltersNoAddRef() { return Filters; }

	// method
	virtual void Open(const ttstr & storagename) = 0;
	virtual void Play() = 0;
	virtual void Stop() = 0;
	virtual void SetPos(float x, float y, float z) = 0;

	// properties
    virtual tjs_uint64 GetPosition() = 0;
	virtual void SetPosition(tjs_uint64 pos) = 0;
    virtual tjs_uint64 GetSamplePosition() = 0;
	virtual void SetSamplePosition(tjs_uint64 pos) = 0;
	virtual bool GetPaused() const = 0;
	virtual void SetPaused(bool b) = 0;
	virtual tjs_uint64 GetTotalTime() = 0;
	virtual void SetLooping(bool b) = 0;
	virtual bool GetLooping() const = 0;
	virtual void SetVolume(tjs_int v) = 0;
	virtual tjs_int GetVolume() const = 0;
	virtual void SetVolume2(tjs_int v) = 0;
	virtual tjs_int GetVolume2() const = 0;
	virtual void SetPan(tjs_int v) = 0;
	virtual tjs_int GetPan() const = 0;
	virtual void SetPosX(float v) = 0;
	virtual float GetPosX() const = 0;
	virtual void SetPosY(float v) = 0;
	virtual float GetPosY() const = 0;
	virtual void SetPosZ(float v) = 0;
	virtual float GetPosZ() const = 0;
	virtual tjs_int GetFrequency() const = 0;
	virtual void SetFrequency(tjs_int freq) = 0;
	virtual tjs_int GetBitsPerSample() const = 0;
	virtual tjs_int GetChannels() const = 0;
};
#endif
//---------------------------------------------------------------------------





//---------------------------------------------------------------------------
// tTJSNC_WaveSoundBuffer : TJS WaveSoundBuffer class
//---------------------------------------------------------------------------
#if 0
class tTJSNC_WaveSoundBuffer : public tTJSNativeClass
{
public:
	tTJSNC_WaveSoundBuffer();
	static tjs_uint32 ClassID;

	typedef tTJSNativeInstance *(*FuncCreateNativeInstance)();
	FuncCreateNativeInstance Factory;
protected:
	tTJSNativeInstance *CreateNativeInstance() { return Factory(); }
};
//---------------------------------------------------------------------------
extern tTJSNativeClass * TVPCreateNativeClass_SoundBuffer();
#endif
//---------------------------------------------------------------------------






//---------------------------------------------------------------------------
// tTJSNI_WaveFlags : Wave Flags object
//---------------------------------------------------------------------------
#if 0
class tTJSNI_WaveFlags : public tTJSNativeInstance
{
	typedef tTJSNativeInstance inherited;

	tTJSNI_BaseWaveSoundBuffer * Buffer;

public:
	tTJSNI_WaveFlags();
	~tTJSNI_WaveFlags();
	tjs_error TJS_INTF_METHOD Construct(tjs_int numparams, tTJSVariant **param,
		iTJSDispatch2 *tjs_obj);
	void TJS_INTF_METHOD Invalidate();

	tTJSNI_BaseWaveSoundBuffer * GetBuffer() const { return Buffer; }
};
#endif
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
// tTJSNC_WaveFlags : Wave Flags class
//---------------------------------------------------------------------------
#if 0
class tTJSNC_WaveFlags : public tTJSNativeClass
{
public:
	tTJSNC_WaveFlags();
	static tjs_uint32 ClassID;

protected:
	tTJSNativeInstance *CreateNativeInstance() { return new tTJSNI_WaveFlags(); }
};
//---------------------------------------------------------------------------
iTJSDispatch2 * TVPCreateWaveFlagsObject(iTJSDispatch2 * buffer);
#endif
//---------------------------------------------------------------------------






#endif
