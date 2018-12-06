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

private:

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
