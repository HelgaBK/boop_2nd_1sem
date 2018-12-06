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

	void writeNote(Zamitka* const zamitka, HPDF_Doc& pdf, HPDF_Page& page, HPDF_Font& def_font) {
		HPDF_Font regularFont = HPDF_GetFont(pdf, font_list[0], NULL);
		HPDF_Font italicFont = HPDF_GetFont(pdf, font_list[1], NULL);

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

	void pushText(string const & text, HPDF_Doc& pdf, HPDF_Page& page,
		int const shift, int const maxLength, HPDF_Font font, int fontSize) {
		int strLen = text.size(), charStringSize = 0;
		string charString;
		for (int i = 0; i < strLen; i++) {
			if ((text[i] == '\n') || (charString.size() >= maxLength)) {
				HPDF_Page_ShowText(page, castStringToChar(charString));
				HPDF_Page_MoveTextPos(page, 0, shift);
				charString = "";
			}
			else
				charString += text[i];
		}
		if (charString.size() > 0) {
			HPDF_Page_ShowText(page, castStringToChar(charString));
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

const char *font_list[] = {
	"Courier",
	"Courier-Bold",
	"Courier-Oblique",
	"Courier-BoldOblique",
	"Helvetica",
	"Helvetica-Bold",
	"Helvetica-Oblique",
	"Helvetica-BoldOblique",
	"Times-Roman",
	"Times-Bold",
	"Times-Italic",
	"Times-BoldItalic",
	"Symbol",
	"ZapfDingbats",
	NULL
};

int main(int argc, char **argv) {


	return 0;
}
