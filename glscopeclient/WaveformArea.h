/***********************************************************************************************************************
*                                                                                                                      *
* ANTIKERNEL v0.1                                                                                                      *
*                                                                                                                      *
* Copyright (c) 2012-2019 Andrew D. Zonenberg                                                                          *
* All rights reserved.                                                                                                 *
*                                                                                                                      *
* Redistribution and use in source and binary forms, with or without modification, are permitted provided that the     *
* following conditions are met:                                                                                        *
*                                                                                                                      *
*    * Redistributions of source code must retain the above copyright notice, this list of conditions, and the         *
*      following disclaimer.                                                                                           *
*                                                                                                                      *
*    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the       *
*      following disclaimer in the documentation and/or other materials provided with the distribution.                *
*                                                                                                                      *
*    * Neither the name of the author nor the names of any contributors may be used to endorse or promote products     *
*      derived from this software without specific prior written permission.                                           *
*                                                                                                                      *
* THIS SOFTWARE IS PROVIDED BY THE AUTHORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED   *
* TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL *
* THE AUTHORS BE HELD LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES        *
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR       *
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT *
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE       *
* POSSIBILITY OF SUCH DAMAGE.                                                                                          *
*                                                                                                                      *
***********************************************************************************************************************/

/**
	@file
	@author Andrew D. Zonenberg
	@brief  Declaration of WaveformArea
 */
#ifndef WaveformArea_h
#define WaveformArea_h

#include "WaveformGroup.h"

float sinc(float x, float width);
float blackman(float x, float width);

class OscilloscopeWindow;

class WaveformArea : public Gtk::GLArea
{
public:
	WaveformArea(Oscilloscope* scope, OscilloscopeChannel* channel, OscilloscopeWindow* parent);
	WaveformArea(const WaveformArea* clone);
	virtual ~WaveformArea();

	void OnWaveformDataReady();

	OscilloscopeChannel* GetChannel()
	{ return m_channel; }

	void ClearPersistence()
	{ m_persistenceClear = true; }

	WaveformGroup* m_group;

	//Helpers for figuring out what kind of signal our primary trace is
	bool IsEye();
	bool IsWaterfall();
	bool IsFFT();

protected:
	void SharedCtorInit();

	virtual void on_realize();
	virtual void on_unrealize();
	virtual void on_resize (int width, int height);
	virtual bool on_render(const Glib::RefPtr<Gdk::GLContext>& context);
	virtual bool on_button_press_event(GdkEventButton* event);
	virtual bool on_button_release_event(GdkEventButton* event);
	virtual bool on_scroll_event (GdkEventScroll* ev);
	virtual bool on_motion_notify_event(GdkEventMotion* event);

	void CreateWidgets();

	//Context menu
	Gtk::Menu m_contextMenu;
		Gtk::CheckMenuItem m_persistenceItem;
		Gtk::MenuItem m_cursorItem;
			Gtk::Menu m_cursorMenu;
				Gtk::RadioMenuItem::Group m_cursorGroup;
				Gtk::RadioMenuItem m_cursorNoneItem;
				Gtk::RadioMenuItem m_cursorSingleVerticalItem;
				Gtk::RadioMenuItem m_cursorDualVerticalItem;
		Gtk::MenuItem m_moveItem;
			Gtk::Menu m_moveMenu;
				Gtk::MenuItem m_moveNewGroupBelowItem;
				Gtk::MenuItem m_moveNewGroupRightItem;
				std::set<Gtk::MenuItem*> m_moveExistingGroupItems;
		Gtk::MenuItem m_copyItem;
			Gtk::Menu m_copyMenu;
				Gtk::MenuItem m_copyNewGroupBelowItem;
				Gtk::MenuItem m_copyNewGroupRightItem;
				std::set<Gtk::MenuItem*> m_copyExistingGroupItems;
		Gtk::MenuItem m_decodeItem;
			Gtk::Menu m_decodeMenu;
				Gtk::MenuItem m_decodeAnalysisItem;
					Gtk::Menu m_decodeAnalysisMenu;
				Gtk::MenuItem m_decodeClockItem;
					Gtk::Menu m_decodeClockMenu;
				Gtk::MenuItem m_decodeConversionItem;
					Gtk::Menu m_decodeConversionMenu;
				Gtk::MenuItem m_decodeMathItem;
					Gtk::Menu m_decodeMathMenu;
				Gtk::MenuItem m_decodeMiscItem;
					Gtk::Menu m_decodeMiscMenu;
				Gtk::MenuItem m_decodeSerialItem;
					Gtk::Menu m_decodeSerialMenu;
		Gtk::MenuItem m_measureItem;
			Gtk::Menu m_measureMenu;
				Gtk::MenuItem m_measureVertItem;
					Gtk::Menu m_measureVertMenu;
				Gtk::MenuItem m_measureHorzItem;
					Gtk::Menu m_measureHorzMenu;
		Gtk::MenuItem m_triggerItem;
			Gtk::Menu m_triggerMenu;
			Gtk::RadioMenuItem::Group m_triggerGroup;
			Gtk::RadioMenuItem m_risingTriggerItem;
			Gtk::RadioMenuItem m_fallingTriggerItem;
			Gtk::RadioMenuItem m_bothTriggerItem;
		Gtk::MenuItem m_couplingItem;
			Gtk::Menu m_couplingMenu;
				Gtk::RadioMenuItem::Group m_couplingGroup;
				Gtk::RadioMenuItem m_dc50CouplingItem;
				Gtk::RadioMenuItem m_dc1MCouplingItem;
				Gtk::RadioMenuItem m_ac1MCouplingItem;
				Gtk::RadioMenuItem m_gndCouplingItem;
		Gtk::MenuItem m_attenItem;
			Gtk::Menu m_attenMenu;
				Gtk::RadioMenuItem::Group m_attenGroup;
				Gtk::RadioMenuItem m_atten1xItem;
				Gtk::RadioMenuItem m_atten10xItem;
				Gtk::RadioMenuItem m_atten20xItem;
		Gtk::MenuItem m_bwItem;
			Gtk::Menu m_bwMenu;
				Gtk::RadioMenuItem::Group m_bwGroup;
				Gtk::RadioMenuItem m_bwFullItem;
				Gtk::RadioMenuItem m_bw200Item;
				Gtk::RadioMenuItem m_bw20Item;
	void UpdateContextMenu();
	void UpdateMeasureContextMenu(std::vector<Widget*> children);
	bool m_updatingContextMenu;
	void OnHide();
	void OnTogglePersistence();
	void OnProtocolDecode(std::string name);
	void OnMeasure(std::string name);
	void OnTriggerMode(Oscilloscope::TriggerType type, Gtk::RadioMenuItem* item);
	void OnBandwidthLimit(int mhz, Gtk::RadioMenuItem* item);
	void OnMoveNewRight();
	void OnMoveNewBelow();
	void OnMoveToExistingGroup(WaveformGroup* group);
	void OnCopyNewRight();
	void OnCopyNewBelow();
	void OnCopyToExistingGroup(WaveformGroup* group);
	void OnCursorConfig(WaveformGroup::CursorConfig config, Gtk::RadioMenuItem* item);

	void CleanupBufferObjects();

	void RefreshMeasurements();

	int m_width;
	int m_height;

	float m_cursorX;
	float m_cursorY;

	//Display options
	bool m_persistence;
	bool m_persistenceClear;

	//GL stuff (TODO organize)
	Program m_waveformProgram;
	Framebuffer m_windowFramebuffer;

	//Trace rendering
	bool PrepareGeometry();
	void RenderTrace();
	void InitializeWaveformPass();
	std::vector<VertexBuffer*> m_traceVBOs;
	std::vector<VertexArray*> m_traceVAOs;
	glm::mat4 m_projection;
	size_t m_waveformLength;
	std::vector<float> m_traceBuffer;
	float m_xoff;

	//Color correction
	void RenderTraceColorCorrection();
	void InitializeColormapPass();
	VertexArray m_colormapVAO;
	VertexBuffer m_colormapVBO;
	Program m_colormapProgram;
	Framebuffer m_waveformFramebuffer;			//multisample
	Texture m_waveformTexture;
	Framebuffer m_waveformFramebufferResolved;	//single sample
	Texture m_waveformTextureResolved;

	//Persistence
	void RenderPersistenceOverlay();
	void InitializePersistencePass();
	Program m_persistProgram;
	VertexArray m_persistVAO;
	VertexBuffer m_persistVBO;

	//Eye pattern rendering
	void RenderEye();
	void InitializeEyePass();
	Program m_eyeProgram;
	VertexArray m_eyeVAO;
	VertexBuffer m_eyeVBO;
	Texture m_eyeTexture;
	Texture m_eyeColorRamp[6];

	//Waterfall rendering
	void RenderWaterfall();

	//Cairo overlay rendering for text and protocol decode overlays
	void RenderCairoUnderlays();
	void DoRenderCairoUnderlays(Cairo::RefPtr< Cairo::Context > cr);
	void RenderBackgroundGradient(Cairo::RefPtr< Cairo::Context > cr);
	void RenderGrid(Cairo::RefPtr< Cairo::Context > cr);
	void RenderCairoOverlays();
	void DoRenderCairoOverlays(Cairo::RefPtr< Cairo::Context > cr);
	void RenderCursors(Cairo::RefPtr< Cairo::Context > cr);
	void RenderChannelLabel(Cairo::RefPtr< Cairo::Context > cr);
	void RenderDecodeOverlays(Cairo::RefPtr< Cairo::Context > cr);
	void InitializeCairoPass();
	Texture m_cairoTexture;
	Texture m_cairoTextureOver;
	VertexArray m_cairoVAO;
	VertexBuffer m_cairoVBO;
	Program m_cairoProgram;

	void ResetTextureFiltering();

	//Math helpers
	float PixelsToVolts(float pix);
	float VoltsToPixels(float volt);
	float VoltsToYPosition(float volt);
	float YPositionToVolts(float y);
	float DbToYPosition(float db);
	int64_t PixelsToPicoseconds(float pix);
	float PicosecondsToPixels(int64_t t);
	float PicosecondsToXPosition(int64_t t);
	int64_t XPositionToPicoseconds(float pix);

	Oscilloscope* m_scope;
	OscilloscopeChannel* m_channel;
	OscilloscopeChannel* m_selectedChannel;
	OscilloscopeWindow* m_parent;

	std::vector<ProtocolDecoder*> m_overlays;
	std::map<ProtocolDecoder*, int> m_overlayPositions;

	double m_lastFrameStart;
	double m_frameTime;
	long m_frameCount;

	float m_pixelsPerVolt;
	float m_padding;
	float m_plotRight;

	enum ClickLocation
	{
		LOC_PLOT,
		LOC_VSCALE,
		LOC_TRIGGER
	} m_clickLocation;

	ClickLocation HitTest(double x, double y);

	enum DragStates
	{
		DRAG_NONE,
		DRAG_TRIGGER,
		DRAG_CURSOR
	} m_dragState;

	bool	m_msaaEnabled;
};

#endif
