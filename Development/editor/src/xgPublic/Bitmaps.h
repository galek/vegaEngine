/*=============================================================================
Bitmaps.h: Classes based on standard bitmaps

Revision history:
* Created by Warren Marshall

=============================================================================*/
#pragma once

namespace vega
{
	/*-----------------------------------------------------------------------------
	WxBitmap.

	A bitmap that has OS specific loading hooks.
	-----------------------------------------------------------------------------*/

	class WxBitmap : public wxBitmap
	{
	public:
		WxBitmap();
		WxBitmap(const wxImage& img, int depth = -1);
		WxBitmap(int width, int height, int depth = -1);
		WxBitmap(wxString InFilename);
		~WxBitmap();

		virtual bool Load(wxString InFilename);
		virtual bool LoadLiteral(wxString InFilename);
	};

	/*-----------------------------------------------------------------------------
	WxMaskedBitmap.

	A bitmap that automatically generates a wxMask for itself.
	-----------------------------------------------------------------------------*/

	class WxMaskedBitmap : public WxBitmap
	{
	public:
		WxMaskedBitmap();
		WxMaskedBitmap(wxString InFilename);
		~WxMaskedBitmap();

		virtual bool Load(wxString InFilename);
	};

	/*-----------------------------------------------------------------------------
	WxAlphaBitmap.

	A bitmap that automatically generates a wxBitmap for itself based on
	the alpha channel of the source TGA file.
	-----------------------------------------------------------------------------*/

	class WxAlphaBitmap : public WxBitmap
	{
	public:
		WxAlphaBitmap();
		WxAlphaBitmap(wxString InFilename, bool InBorderBackground);
		~WxAlphaBitmap();

	private:
		wxImage LoadAlpha(wxString InFilename, bool InBorderBackground);
	};
}

/*-----------------------------------------------------------------------------
The End.
-----------------------------------------------------------------------------*/