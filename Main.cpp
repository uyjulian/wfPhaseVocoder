

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <objidl.h>
#endif
#include "tp_stub.h"

#define EXPORT(hr) extern "C" __declspec(dllexport) hr __stdcall

#include "PhaseVocoderFilter.h"

//---------------------------------------------------------------------------
#ifdef _WIN32
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved) {
	return 1;
}
#endif
//---------------------------------------------------------------------------
static tjs_int GlobalRefCountAtInit = 0;
EXPORT(HRESULT) V2Link(iTVPFunctionExporter *exporter)
{
	// スタブの初期化(必ず記述する)
	TVPInitImportStub(exporter);

	// TJS のグローバルオブジェクトを取得する
	iTJSDispatch2 * global = TVPGetScriptDispatch();

	if (global)
	{
		tTJSVariant val;
		tjs_error er;

		er = global->PropGet(TJS_MEMBERMUSTEXIST, TJS_W("WaveSoundBuffer"), nullptr, &val, global);
		if (TJS_SUCCEEDED(er) && (val.Type() == tvtObject))
		{
			tTJSVariantClosure clo = val.AsObjectClosureNoAddRef();
			if (clo.Object)
			{
				iTJSDispatch2 * tjsclass = TVPCreateNativeClass_PhaseVocoder();
				val = tTJSVariant(tjsclass);
				tjsclass->Release();
				clo.PropSet(TJS_MEMBERENSURE, TJS_W("PhaseVocoder"), nullptr, &val, nullptr);
			}
		}
	}

	// - global を Release する
	if (global)
	{
		global->Release();
	}

	// この時点での TVPPluginGlobalRefCount の値を
	GlobalRefCountAtInit = TVPPluginGlobalRefCount;
	// として控えておく。TVPPluginGlobalRefCount はこのプラグイン内で
	// 管理されている tTJSDispatch 派生オブジェクトの参照カウンタの総計で、
	// 解放時にはこれと同じか、これよりも少なくなってないとならない。
	// そうなってなければ、どこか別のところで関数などが参照されていて、
	// プラグインは解放できないと言うことになる。

	return TJS_S_OK;
}
//---------------------------------------------------------------------------
EXPORT(HRESULT) V2Unlink()
{
	// 吉里吉里側から、プラグインを解放しようとするときに呼ばれる関数。

	// もし何らかの条件でプラグインを解放できない場合は
	// この時点で E_FAIL を返すようにする。
	// ここでは、TVPPluginGlobalRefCount が GlobalRefCountAtInit よりも
	// 大きくなっていれば失敗ということにする。
	if(TVPPluginGlobalRefCount > GlobalRefCountAtInit) return TJS_E_FAIL;
		// E_FAIL が帰ると、Plugins.unlink メソッドは偽を返す

	/*
		ただし、クラスの場合、厳密に「オブジェクトが使用中である」ということを
		知るすべがありません。基本的には、Plugins.unlink によるプラグインの解放は
		危険であると考えてください (いったん Plugins.link でリンクしたら、最後ま
		でプラグインを解放せず、プログラム終了と同時に自動的に解放させるのが吉)。
	*/

	// プロパティ開放
	// - まず、TJS のグローバルオブジェクトを取得する
	iTJSDispatch2 * global = TVPGetScriptDispatch();

	// メニューは解放されないはずなので、明示的には解放しない

	// - global の DeleteMember メソッドを用い、オブジェクトを削除する
	if (global)
	{
		tTJSVariant val;
		tjs_error er;

		er = global->PropGet(TJS_MEMBERMUSTEXIST, TJS_W("WaveSoundBuffer"), NULL, &val, global);
		if (TJS_SUCCEEDED(er) && (val.Type() == tvtObject))
		{
			tTJSVariantClosure clo = val.AsObjectClosureNoAddRef();
			if (clo.Object)
			{
				clo.DeleteMember(0, TJS_W("PhaseVocoder"), nullptr, nullptr);
			}
		}
	}

	if (global)
	{
		global->Release();
	}

	// スタブの使用終了(必ず記述する)
	TVPUninitImportStub();

	return TJS_S_OK;
}
//---------------------------------------------------------------------------
