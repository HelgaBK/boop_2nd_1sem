#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <setjmp.h>
#include <iostream>
#include <fstream>
#include <iomanip> 
#include <vector>
#include <string>
#include <cstring>
#include "hpdf.h"
#include "json.hpp"

jmp_buf env;

using std::string;
using std::vector;

using nlohmann::json;

#ifdef HPDF_DLL
void  __stdcall
#else
void
#endif
error_handler(HPDF_STATUS   error_no,
	HPDF_STATUS   detail_no,
	void         *user_data) {
	printf("ERROR: error_no=%04X, detail_no=%u\n", (HPDF_UINT)error_no,
		(HPDF_UINT)detail_no);
	longjmp(env, 1);
}

const char *font_list[] = {
	"Times-Roman",
	"Times-Italic",
	NULL
};

char const *encoding = { "ISO8859-5" };

struct Zamitka {
	string text;
	vector<string> tags;
	string time; // <3

	Zamitka(string text, vector<string> tags, string time) {
		this->tags = tags;
		this->text = text;
		this->time = time;
	}
};

class YaRobliyPDF {
public:

	void readJSON(string jsonFile) {
		std::ifstream jsonInput(jsonFile);
		json jsonObject;
		jsonInput >> jsonObject;

		json zamitkasJson = jsonObject["notes"];
		json archiveJson = jsonObject["archive"];
		json tagsJson = jsonObject["tags"];

		readSingleZamitka(this->zamitkas, zamitkasJson);
		readSingleZamitka(this->archivedZamitkas, archiveJson);
		this->tags = tagsJson.get<vector<string>>();
	}

	bool writePDF(string filepath = "archive.pdf", string owner = "", string user = "") {
		const char *notes = "Notes";
		const char *archivedNotes = "Archived Notes";
		const char *tagsText = "Tags";
		HPDF_Doc  pdf;
		char fname[256];
		strcpy(fname, filepath.c_str());
		HPDF_Page page;
		HPDF_Font def_font;
		HPDF_REAL tw;
		HPDF_REAL height;
		HPDF_REAL width;

		pdf = HPDF_New(error_handler, NULL);
		HPDF_UseUTFEncodings(pdf);
		HPDF_SetCurrentEncoder(pdf, "UTF-8");
		if (!pdf) {
			printf("error: cannot create PdfDoc object\n");
			return 1;
		}

		if (setjmp(env)) {
			HPDF_Free(pdf);
			return 1;
		}

		/* Add a new page object. */
		page = HPDF_AddPage(pdf);

		height = HPDF_Page_GetHeight(page);
		width = HPDF_Page_GetWidth(page);

		// Notes text
		def_font = HPDF_GetFont(pdf, font_list[0], encoding);
		HPDF_Page_SetFontAndSize(page, def_font, 24);

		tw = HPDF_Page_TextWidth(page, notes);
		HPDF_Page_BeginText(page);
		HPDF_Page_TextOut(page, (width - tw) / 2, height - 50, notes);

		// Write zaitkas
		HPDF_Page_MoveTextPos(page, -((width - tw) / 2) + 60, -20);
		for (int i = 0; i < this->zamitkas.size(); i++)
			writeNote(this->zamitkas[i], pdf, page, def_font);

		// Archved notes text
		HPDF_Page_SetFontAndSize(page, def_font, 24);

		tw = HPDF_Page_TextWidth(page, archivedNotes);
		HPDF_Page_MoveTextPos(page, 60, -30);
		HPDF_Page_TextOut(page, (width - tw) / 2, HPDF_Page_GetCurrentTextPos(page).y, archivedNotes);

		// Write archived zamitkas
		HPDF_Page_MoveTextPos(page, -((width - tw) / 2) + 60, -20);
		for (int i = 0; i < this->archivedZamitkas.size(); i++)
			writeNote(this->archivedZamitkas[i], pdf, page, def_font);

		// Tags text
		HPDF_Page_SetFontAndSize(page, def_font, 24);

		tw = HPDF_Page_TextWidth(page, tagsText);
		HPDF_Page_MoveTextPos(page, 60, -30);
		HPDF_Page_TextOut(page, (width - tw) / 2, HPDF_Page_GetCurrentTextPos(page).y, tagsText);

		//	Write tags
		HPDF_Page_MoveTextPos(page, -((width - tw) / 2) + 60, -20);
		HPDF_Font regularFont = HPDF_GetFont(pdf, font_list[0], encoding);
		HPDF_Font italicFont = HPDF_GetFont(pdf, font_list[1], encoding);
		HPDF_Page_SetFontAndSize(page, regularFont, 14);
		for (int i = 0; i < this->tags.size(); i++)
			pushText(this->tags[i], pdf, page, -15, 57, regularFont, 14);
		HPDF_Page_EndText(page);

		// Set passwords
		if (user != "" && owner != "")
			HPDF_SetPassword(pdf, castStringToChar(owner), castStringToChar(user));

		HPDF_SaveToFile(pdf, fname);

		/*	CLEAN  */
		HPDF_Free(pdf);
		return true;
	}

	void writeNote(Zamitka* const zamitka, HPDF_Doc& pdf, HPDF_Page& page, HPDF_Font& def_font) {
		HPDF_Font regularFont = HPDF_GetFont(pdf, font_list[0], encoding);
		HPDF_Font italicFont = HPDF_GetFont(pdf, font_list[1], encoding);

		//text
		HPDF_Page_SetFontAndSize(page, regularFont, 14);
		pushText(zamitka->text, pdf, page, -15, 57, regularFont, 14);

		HPDF_Page_SetFontAndSize(page, italicFont, 10);
		//tags
		string tags = "Tags : ";
		for (int i = 0; i < zamitka->tags.size() - 1; i++)
			tags += zamitka->tags[i] + "; ";
		tags += zamitka->tags[zamitka->tags.size() - 1];
		pushText(tags, pdf, page, -11, 80, italicFont, 10);

		//time
		string time = "Last edited time : " + zamitka->time;
		pushText(time, pdf, page, -11, 80, italicFont, 10);

		string divider = "_";
		for (int i = 0; i < 80; i++)
			divider += '_';
		pushText(divider, pdf, page, -11, 80, italicFont, 10);

		HPDF_Page_MoveTextPos(page, 0, -20);
	}

private:

	HPDF_Page checkForNewPage(HPDF_Doc& pdf, HPDF_Page& page, int height, HPDF_Font font, int fontSize) const {
		HPDF_Point pointerPosition = HPDF_Page_GetCurrentTextPos(page);
		HPDF_Page newPage;
		if (pointerPosition.y < (fontSize + height) * 3) {
			newPage = HPDF_AddPage(pdf);
			HPDF_Page_SetFontAndSize(newPage, font, fontSize);
			HPDF_Page_EndText(page);
			HPDF_Page_BeginText(newPage);
			HPDF_REAL pageHeight;
			HPDF_REAL pageWidth;
			HPDF_Point pointerPositionOnSecondPage = HPDF_Page_GetCurrentTextPos(newPage);
			pageHeight = HPDF_Page_GetHeight(newPage);
			pageWidth = HPDF_Page_GetWidth(newPage);
			HPDF_Page_MoveTextPos(newPage, 60, pageHeight - height);
			return newPage;
		}
		return page;
	}

	void pushText(string const & text, HPDF_Doc& pdf, HPDF_Page& page,
		int const shift, int const maxLength, HPDF_Font font, int fontSize) {
		int strLen = text.size(), charStringSize = 0;
		string charString;
		for (int i = 0; i < strLen; i++) {
			if ((text[i] == '\n') || (charString.size() >= maxLength)) {
				HPDF_Page_ShowText(page, castStringToChar(charString));
				page = checkForNewPage(pdf, page, (-shift) + 1, font, fontSize);
				HPDF_Page_MoveTextPos(page, 0, shift);
				charString = "";
			}
			else
				charString += text[i];
		}
		if (charString.size() > 0) {
			HPDF_Page_ShowText(page, castStringToChar(charString));
			page = checkForNewPage(pdf, page, (-shift) + 1, font, fontSize);
			HPDF_Page_MoveTextPos(page, 0, shift);
		}
	}

	void readSingleZamitka(vector<Zamitka*> & zamitky, json singleJson) {
		vector<json> zamitkasList;
		zamitkasList = singleJson.get<vector<json>>();
		for (int i = 0; i < zamitkasList.size(); i++) {
			Zamitka* sn = new Zamitka(
				zamitkasList[i]["text"].get<string>(),
				zamitkasList[i]["tags"].get<vector<string>>(),
				zamitkasList[i]["time"].get<string>());
			zamitky.push_back(sn);
		}
	}

	char* const castStringToChar(string toConvert) {
		char *arr;
		arr = new char[toConvert.size()];
		strcpy(arr, toConvert.c_str());
		return arr;
	}

	vector<Zamitka *> zamitkas;
	vector<Zamitka *> archivedZamitkas;
	vector<string> tags;
};

int main(int argc, char **argv) {
	YaRobliyPDF yrp;
	yrp.readJSON("data.json");

	if (argc == 2)
		yrp.writePDF(argv[1]);
	else if (argc == 4) {
		yrp.writePDF(argv[1], argv[2], argv[3]);
	}
	else return 1;
	return 0;
}
