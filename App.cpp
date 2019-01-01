// cd /home/dsv/Downloads/mm1/iimavlib-master/build
// ./bin/app ../data/drum0.wav ../data/drum1.wav ../data/drum2.wav ../data/min_kick_17_F.wav ../data/Hip-Hop-Snare-1.wav

#include "iimavlib/SDLDevice.h"
#include "iimavlib/WaveFile.h"
#include "iimavlib/AudioFilter.h"
#include "iimavlib_high_api.h"
#ifdef SYSTEM_LINUX
#include <unistd.h>
#endif
#include <algorithm>
#include <vector>
#include "iimavlib/Utils.h"
#include "iimavlib/video_ops.h"
#include <stdlib.h>     /* srand, rand */
#include <sys/timeb.h>
#include <utility>      // std::pair, std::make_pair
#include <string>       // std::string
#include <list>       // std::list
#include <thread>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <math.h>

using std::unique_ptr;

using namespace iimavlib;

class Context;
class SoundControl;
class App;

int A[]		=		{1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1};
int B[]		=		{1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0};
int C[]		=		{0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1};
int D[]		=		{1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0};
int E[]		=		{1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1};
int F[]		=		{1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0};
int G[]		=		{0, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1};
int H[]		=		{1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1};
int I[]		=		{1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1};
int J[]		=		{0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1};
int K[]		=		{1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1};
int L[]		=		{1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1};
int M[]		=		{1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1};
int N[]		=		{1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1};
int O[]		=		{1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1};
int P[]		=		{1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0};
int Q[]		=		{1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1};
int R[]		=		{1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1};
int S[]		=		{0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0};
int T[]		=		{1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0};
int U[]		=		{1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1};
int V[]		=		{1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0};
int W[]		=		{1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1};
int X[]		=		{1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1};
int Y[]		=		{1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0};
int Z[]		=		{1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1};
int N0[]	=		{1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1};
int N1[]	=		{0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1};
int N2[]	=		{1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1};
int N3[]	=		{1, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1};
int N4[]	=		{1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1};
int N5[]	=		{1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0};
int N6[]	=		{0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1};
int N7[]	=		{1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0};
int N8[]	=		{1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1};
int N9[]	=		{1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0};
int SP[]	=		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int MINUS[]	=		{0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0};
int PLUS[]	=		{0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0};
int COLON[]	=		{0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0};
int UNDERSCORE[]=	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1};

int WIN_W = 1024;
int WIN_H = 768;

int SIZE_LINE_BOLDEST = 5;
int SIZE_LINE_BOLD = 3;
int SIZE_LINE_THIN = 1;

rgb_t COLOR_BG = rgb_t(25, 25, 25);
rgb_t COLOR_BG_LIGHTER = rgb_t(35, 35, 35);
rgb_t COLOR_LINE = rgb_t(255, 0, 0);
rgb_t COLOR_LINE_INACTIVE = rgb_t(150, 0, 0);

float DEFAULT_VOLUME = 0.5f;

SoundControl* G_SC = nullptr;

bool isIn(int x, int y, int r_x, int r_y, int r_width, int r_height) {
	return x > r_x && x < r_x + r_width && y > r_y && y < r_y + r_height;
}
bool isIn(int x, int y, rectangle_t rect) {
	return isIn(x, y, rect.x, rect.y, rect.width, rect.height);
}

class Drawable {
public:
	virtual void draw(Context& ctx) = 0;
};

class Updatable {
public:
	virtual void update(int delta) = 0;
};

class Clickable {
public:
	virtual rectangle_t hitrectangle() = 0;
	virtual void click(int x, int y, int b) = 0;
};

class Context {
public:
	SDLDevice* sdl;
	video_buffer_t* data;
	int width, height;
	Context(SDLDevice& sdl_c, video_buffer_t& data_c) {
		sdl = &sdl_c;
		data = &data_c;
		width = data_c.size.width;
		height = data_c.size.height;
	}
	void draw(Drawable& drawable) {
		drawable.draw(*this);
	}
	int render() {
		return sdl->blit(*data);
	}
	void clear(rgb_t color) {data->clear(color);}
	void pixel(int x, int y, rgb_t color) {(*data)(x, y) = color;}
	void line(int x0, int y0, int x1, int y1, rgb_t color) {draw_line(*data, rectangle_t(x0, y0), rectangle_t(x1, y1), color);}
	void circle(int x, int y, int r, rgb_t color) {circle(x, y, 2 * r, 2 * r, color);}
	void circle(int x, int y, int width, int height, rgb_t color) {draw_circle(*data, rectangle_t(x, y, x + width, y + height), color);}
	void rectangle(int x, int y, int width, int height, rgb_t color) {draw_rectangle(*data, rectangle_t(x, y, width, height), color);}
	void emptyRectangle(int x, int y, int width, int height, int border, rgb_t color) {draw_empty_rectangle(*data, rectangle_t(x, y, width, height), border, color);}
	void emptyRectangle(int x, int y, int width, int height, rgb_t color) {emptyRectangle(x, y, width, height, 1, color);}
	void polyline(const std::vector<rectangle_t>& points, rgb_t color) {draw_polyline(*data, points, color);}
	void polygon(const std::vector<rectangle_t>& points, rgb_t color) {draw_polyline(*data, points, color);}
	void triangle(int x1, int y1, int x2, int y2, int x3, int y3, rgb_t color) {
		std::vector<rectangle_t> v;
		v.push_back(rectangle_t(x1, y1, 0, 0));
		v.push_back(rectangle_t(x2, y2, 0, 0));
		v.push_back(rectangle_t(x3, y3, 0, 0));
		v.push_back(rectangle_t(x1, y1, 0, 0));
		polygon(v, color);
	};
	//void draw_line_steep(iimavlib::video_buffer_t& data, iimavlib::rectangle_t start, iimavlib::rectangle_t end, iimavlib::rgb_t color)
	//void draw_line_thick(iimavlib::video_buffer_t& data, iimavlib::rectangle_t start, iimavlib::rectangle_t end, int border, iimavlib::rgb_t color)
};

class SampleConfig {
public:
	float volume;
	bool muted;

	SampleConfig(): volume(DEFAULT_VOLUME), muted(false) {}

};

class SampleTrack {
public:
	SampleConfig* config;
	std::vector<audio_sample_t>* sampleData;
	int position;

	SampleTrack(): position(0) {

	}

	SampleTrack(std::vector<audio_sample_t>& sampleDatac): position(0) {
		config = new SampleConfig();
		sampleData = new std::vector<audio_sample_t>(sampleDatac.size());
		for (size_t i = 0; i < sampleDatac.size(); i++) {
			sampleData->at(i) = sampleDatac[i];
		}
	}

	SampleTrack* shallowClone() {
		SampleTrack* sampleTrack = new SampleTrack();
		sampleTrack->sampleData = sampleData;
		sampleTrack->position = position;
		sampleTrack->config = config;
		return sampleTrack;
	}

	audio_sample_t getCurrentSample() {
		if (finished() || config->muted) return audio_sample_t();
		audio_sample_t sample = sampleData->at(position);
		float samplefLeft = sample.left / 32768.0f;
		float samplefRight = sample.right / 32768.0f;
		// change the volume
		samplefLeft *= config->volume;
		samplefRight *= config->volume;
		// hard clipping
		if (samplefLeft > 1.0f) samplefLeft = 1.0f;
		if (samplefLeft < -1.0f) samplefLeft = -1.0f;
		if (samplefRight > 1.0f) samplefRight = 1.0f;
		if (samplefRight < -1.0f) samplefRight = -1.0f;
		sample.left = samplefLeft * 32768.0f;
		sample.right = samplefRight * 32768.0f;
		return sample;
	}

	size_t size() {
		return sampleData->size();
	}

	bool finished() {
		return position >= size();
	}
};

class SoundControl: public AudioFilter {
public:
	std::vector<SampleTrack> sampleTracks;
	float masterVolume = 0.8f;

	SoundControl(std::vector<std::string> filesc): AudioFilter(pAudioFilter()) {
		for (int i = 0; i < filesc.size(); i++) {
			loadFileData(filesc[i]);
		}
		G_SC = this;
	}

	bool loadFileData(const std::string filename) {
		try {
			size_t timeDurationRestriction = 120; // 60 sec
			size_t samples = 44100;
			size_t length = samples * timeDurationRestriction;

			WaveFile wav(filename);
			std::vector<audio_sample_t> data(length);
			wav.read_data(data, length);
			data.resize(length);

			sampleTracks.push_back(SampleTrack(data));
			printf("File loaded %s: rate_enum_index=%d\n", filename.c_str(), wav.get_params().rate/*, params.format, params.num_channels*/);
		}
		catch (std::exception &e) {
			printf("Could not load file %s\n", filename.c_str());
			return false;
		}
		return true;
	}

	void playSample(int index) {
		playingSampleTracks.push_back(sampleTracks[index].shallowClone());
	}

	void setVolume(int index, float vol) {
		sampleTracks[index].config->volume = vol;
	}

	void toggleMute(int index) {
		sampleTracks[index].config->muted = !sampleTracks[index].config->muted;
	}

	void forceStop() {
		std::unique_lock<std::mutex> lock(position_mutex);
		playingSampleTracks.clear();
	}

private:
	std::vector<SampleTrack*> playingSampleTracks;

	std::mutex position_mutex;

	audio_sample_t mixSamples(const audio_sample_t& sample1, const audio_sample_t& sample2) {
		float samplef1Left = sample1.left / 32768.0f;
		float samplef2Left = sample2.left / 32768.0f;
		float samplef1Right = sample1.right / 32768.0f;
		float samplef2Right = sample2.right / 32768.0f;
		float mixedLeft = samplef1Left + samplef2Left;
		float mixedRight = samplef1Right + samplef2Right;
		if (mixedLeft > 1.0f) mixedLeft = 1.0f;
		if (mixedLeft < -1.0f) mixedLeft = -1.0f;
		if (mixedRight > 1.0f) mixedRight = 1.0f;
		if (mixedRight < -1.0f) mixedRight = -1.0f;
		audio_sample_t outputSample;
		outputSample.left = mixedLeft * 32768.0f;
		outputSample.right = mixedRight * 32768.0f;
		return outputSample;
	}

	void adjustSampleVolume(audio_sample_t& sample) {
		sample.left *= masterVolume;
		sample.right *= masterVolume;
	}

	error_type_t do_process(audio_buffer_t& buffer) {
		std::unique_lock<std::mutex> lock(position_mutex);
		auto data = buffer.data.begin();
		auto dataEnd = buffer.data.end();
		while (data != dataEnd) {
			if (playingSampleTracks.size() == 0) {
				std::fill(data, dataEnd, 0);
				return error_type_t::ok;
			} else {
				audio_sample_t sampleToWrite;
				for (size_t i = 0; i < playingSampleTracks.size();) {
					sampleToWrite = mixSamples(sampleToWrite, playingSampleTracks[i]->getCurrentSample());
					playingSampleTracks[i]->position++;
					if (playingSampleTracks[i]->finished()) {
						playingSampleTracks.erase(playingSampleTracks.begin() + i);
					} else {
						i++;
					}
				}
				adjustSampleVolume(sampleToWrite);
				*data++ = sampleToWrite;
			}
		}
		return error_type_t::ok;
	}
};

class Text: public Drawable {
public:
	int x, y;
	int size = 2;
	std::string* strRef;
	std::string strStatic;
	size_t maxLength;
	Text(int xc, int yc, std::string* strRefc, size_t maxLengthc = 0) {
		x = xc;
		y = yc;
		strRef = strRefc;
		maxLength = maxLengthc;
	}
	Text(int xc, int yc, std::string strStaticc, size_t maxLengthc = 0) {
		x = xc;
		y = yc;
		strStatic = strStaticc;
		strRef = &strStatic;
		maxLength = maxLengthc;
	}
	size_t length() {
		return strRef->length();
	}
	void draw(Context& ctx) {
		for (size_t i = 0; i < strRef->length(); i++) {
			if (maxLength != 0 && maxLength <= i) break;
			int* arr = A;
			switch ((*strRef).at(i)) {
			case 'a': case 'A': arr = A; break;
			case 'b': case 'B': arr = B; break;
			case 'c': case 'C': arr = C; break;
			case 'd': case 'D': arr = D; break;
			case 'e': case 'E': arr = E; break;
			case 'f': case 'F': arr = F; break;
			case 'g': case 'G': arr = G; break;
			case 'h': case 'H': arr = H; break;
			case 'i': case 'I': arr = I; break;
			case 'j': case 'J': arr = J; break;
			case 'k': case 'K': arr = K; break;
			case 'l': case 'L': arr = L; break;
			case 'm': case 'M': arr = M; break;
			case 'n': case 'N': arr = N; break;
			case 'o': case 'O': arr = O; break;
			case 'p': case 'P': arr = P; break;
			case 'q': case 'Q': arr = Q; break;
			case 'r': case 'R': arr = R; break;
			case 's': case 'S': arr = S; break;
			case 't': case 'T': arr = T; break;
			case 'u': case 'U': arr = U; break;
			case 'v': case 'V': arr = V; break;
			case 'w': case 'W': arr = W; break;
			case 'x': case 'X': arr = X; break;
			case 'y': case 'Y': arr = Y; break;
			case 'z': case 'Z': arr = Z; break;
			case '0': arr = N0; break;
			case '1': arr = N1; break;
			case '2': arr = N2; break;
			case '3': arr = N3; break;
			case '4': arr = N4; break;
			case '5': arr = N5; break;
			case '6': arr = N6; break;
			case '7': arr = N7; break;
			case '8': arr = N8; break;
			case '9': arr = N9; break;
			case ' ': arr = SP; break;
			case '-': arr = MINUS; break;
			case '+': arr = PLUS; break;
			case ':': arr = COLON; break;
			case '_': arr = UNDERSCORE; break;
			default: break;
			}
			for (size_t j = 0; j < 15; j++) {
				if (arr[j]) {
					ctx.rectangle(x + 4 * size * i + (j % 3)*size, y + (j / 3)*size, size, size, rgb_t(255, 255, 255));
				}
			}
		}
	}
};

class CenteredText: public Drawable {
public:
	int x, y, width, height;
	Text* text;
	CenteredText(int xc, int yc, int widthc, int heightc, std::string* strRefc, size_t maxLength = 0) {
		x = xc;
		y = yc;
		width = widthc;
		height = heightc;
		text = new Text(x, y, strRefc, maxLength);
	}
	CenteredText(int xc, int yc, int widthc, int heightc, std::string strStaticc, size_t maxLength = 0) {
		x = xc;
		y = yc;
		width = widthc;
		height = heightc;
		text = new Text(x, y, strStaticc, maxLength);
	}
	void draw(Context& ctx) {
		int textLen = text->size * 3 * text->length() + (text->length() - 1) * text->size;
		text->x = x + (width - textLen) / 2;
		text->y = y + (height - text->size * 5) / 2;
		text->draw(ctx);
	}
};

class Button: public Drawable {
public:
	int x, y, width, height;
	rgb_t color;
	rgb_t fillColor;
	bool highlited = false;
	bool on = false;
	Button(int xc, int yc, int widthc, int heightc, rgb_t colorc, rgb_t fillColorc = COLOR_BG) {
		x = xc;
		y = yc;
		width = widthc;
		height = heightc;
		color = colorc;
		fillColor = fillColorc;
		on = false;
	}
	void draw(Context& ctx) {
		if (highlited) {
			ctx.rectangle(x, y, width, height, rgb_t(color.r / 2, color.g / 2, color.b / 2));
		} else {
			ctx.rectangle(x, y, width, height, fillColor);
		}
		ctx.emptyRectangle(x, y, width, height, (on) ? SIZE_LINE_BOLD : SIZE_LINE_THIN, color);
	}
	rectangle_t hitrectangle() {
		return rectangle_t(x, y, width, height);
	}
	void click(int, int, int) {
		on = !on;
	}
	void light() {
		if (on) highlited = true;
	}
	void dark() {
		highlited = false;
	}
	void toggle() {
		if (highlited) dark(); else light();
	}
};

class HeadControlButton: public Drawable, public Clickable {
public:
	int x, y;
	static const int height = 14;
	static const int width = 14;
	rgb_t bColor = rgb_t(0, 200, 0);
	rgb_t bColorAlt = rgb_t(0, 100, 0);
	bool on = true;
	std::function<void()> action;
	HeadControlButton(int xc, int yc, std::function<void()> actionc) {
		action = actionc;
		x = xc;
		y = yc;
	}
	void draw(Context& ctx) {
		ctx.rectangle(x, y, width, height, (on) ? bColor : bColorAlt);
		ctx.emptyRectangle(x, y, width, height, SIZE_LINE_BOLD, rgb_t(0, 255, 0));
	}
	rectangle_t hitrectangle() {
		return rectangle_t(x, y, width, height);
	}
	void click(int, int, int) {
		on = !on;
		action();
	}
};

class Head: public Drawable {
public:
	int x, y, width, height;
	rgb_t color;
	bool on = true;
	Text text;
	SoundControl* sc;
	HeadControlButton* muteB;
	int index;
	Head(SoundControl* scc, int indexc, int xc, int yc, int widthc, int heightc, std::string& caption, rgb_t colorc):
		text(xc + 8, yc + 10, &caption, 15) {
		sc = scc;
		index = indexc;
		x = xc;
		y = yc;
		width = widthc;
		height = heightc;
		color = colorc;
		muteB = new HeadControlButton(x + width - muteB->width - 8, y + height - muteB->height - 8, [scc, indexc](){scc->toggleMute(indexc);});
	}
	void draw(Context& ctx) {
		ctx.emptyRectangle(x, y, width, height, (on) ? SIZE_LINE_BOLD : SIZE_LINE_THIN, color);
		text.draw(ctx);
		muteB->draw(ctx);
	}
	rectangle_t hitrectangle() {
		return rectangle_t(x, y, width, height);
	}
	void click(int x, int y, int) {
		if (isIn(x, y, muteB->hitrectangle())) {
			muteB->click(x, y, 0);
		}
	}
};

class ButtonCol: public Drawable, public Clickable {
public:
	int x, y;
	size_t count;
	int bSize = 30;
	int bGap = 5;
	rgb_t bColorAlt = rgb_t(175, 0, 0);
	rgb_t lightColor = rgb_t(200, 200, 200);
	std::vector<Button> b;
	bool highlited = false;
	int activeCount = 0;
	ButtonCol(int xc, int yc, int countc, rgb_t fillColor = COLOR_BG) {
		x = xc;
		y = yc;
		count = countc;
		for (size_t i = 0; i < count; i++) {
			b.push_back(Button(x, y + (bSize + bGap) * i, bSize, bSize, COLOR_LINE, fillColor));
		}
	}
	void draw(Context& ctx) {
		for (size_t i = 0; i < count; i++) {
			b.at(i).draw(ctx);
		}
		if (highlited) {
			ctx.emptyRectangle(x, y, bSize, (bSize + bGap) * count - bGap, SIZE_LINE_BOLD, lightColor);
		}
	}
	rectangle_t hitrectangle() {
		return rectangle_t(x, y, bSize, (bSize + bGap) * count);
	}
	void click(int x, int y, int) {
		for (size_t j = 0; j < count; j++) {
			if (isIn(x, y, b.at(j).hitrectangle())) {
				b.at(j).click(x, y, 0);
				if (b.at(j).on) {
					activeCount++;
				}
				else {
					activeCount--;
				}
			}
		}
	}
	void light() {
		highlited = true;
		for (size_t i = 0; i < count; i++) {
			b.at(i).light();
		}
	}
	void dark() {
		highlited = false;
		for (size_t i = 0; i < count; i++) {
			b.at(i).dark();
		}
	}
	bool isEmpty() {
		return activeCount == 0;
	}
};

class HeadCol: public Drawable, public Clickable {
public:
	int x, y;
	size_t count;
	int bHeight = 30;
	int bWidth = 175;
	int bGap = 5;
	rgb_t bColorAlt = rgb_t(175, 0, 0);
	SoundControl* sc;
	std::vector<Head> b;
	HeadCol(SoundControl* scc, int xc, int yc, std::vector<std::string>& captions) {
		sc = scc;
		x = xc;
		y = yc;
		count = captions.size();
		for (size_t i = 0; i < count; i++) {
			b.push_back(Head(scc, i, x, y + (bHeight + bGap) * i, bWidth, bHeight, captions.at(i), COLOR_LINE));
		}
	}
	void draw(Context& ctx) {
		for (size_t i = 0; i < count; i++) {
			b.at(i).draw(ctx);
		}
	}
	rectangle_t hitrectangle() {
		return rectangle_t(x, y, bWidth, (bHeight + bGap) * count);
	}
	void click(int x, int y, int) {
		for (size_t j = 0; j < count; j++) {
			if (isIn(x, y, b.at(j).hitrectangle())) {
				b.at(j).click(x, y, 0);
			}
		}
	}
};

class ValueSpinBtn: public Drawable {
public:
	int x, y, width, height;
	rgb_t color;
	bool up;
	CenteredText* icon;
	ValueSpinBtn(int xc, int yc, int widthc, int heightc, bool upc) {
		x = xc;
		y = yc;
		width = widthc;
		height = heightc;
		up = upc;
		icon = new CenteredText(x, y, width, height, (up) ? "+" : "-");
	}
	void draw(Context& ctx) {
		ctx.emptyRectangle(x, y, width, height, SIZE_LINE_THIN, COLOR_LINE);
		icon->draw(ctx);
	}
	rectangle_t hitrectangle() {return rectangle_t(x, y, width, height);}
};

class ValueSpin: public Drawable, public Updatable, public Clickable {
public:
	int x, y, width = 75, height = 75;
	rgb_t color;
	int* valueRef;
	std::string value;
	int step = 1;
	int bigStep = 5;
	int maxValue = 300;
	int minValue = 1;

	CenteredText* caption;
	ValueSpinBtn* valueSpinBtnUp;
	ValueSpinBtn* valueSpinBtnDown;
	CenteredText* valueText;
	ValueSpin(int xc, int yc, int* valueRefc) {
		x = xc;
		y = yc;
		valueRef = valueRefc;
		value = std::to_string(*valueRef);
		caption = new CenteredText(x, y, width, height / 4, "Tempo");
		valueText = new CenteredText(x, y + 2 * height / 4, width, height / 4, &value);
		valueSpinBtnUp = new ValueSpinBtn(x, y + height / 4, width, height / 4, true);
		valueSpinBtnDown = new ValueSpinBtn(x, y + 3 * height / 4, width, height / 4, false);
	}
	void draw(Context& ctx) {
		caption->draw(ctx);
		valueSpinBtnUp->draw(ctx);
		valueText->draw(ctx);
		valueSpinBtnDown->draw(ctx);
	}
	void update(int) {
		value = std::to_string(*valueRef);
	}
	rectangle_t hitrectangle() {return rectangle_t(x, y, width, height);}
	void click(int x, int y, int b) {
		if (isIn(x, y, valueSpinBtnUp->hitrectangle())) {
			if (b == 0) {
				(*valueRef) += step;
			}
			else {
				(*valueRef) += bigStep;
			}
			if (*valueRef > maxValue) (*valueRef) = maxValue;
		}
		else if (isIn(x, y, valueSpinBtnDown->hitrectangle())) {
			if (b == 0) {
				(*valueRef) -= step;
			}
			else {
				(*valueRef) -= bigStep;
			}
			if (*valueRef < minValue) (*valueRef) = minValue;
		}
	}
};

class ValueHorizontalSpin: public Drawable, public Updatable, public Clickable {
public:
	int x, y, width = 75, height = 30;
	rgb_t color;
	float* valueRef;
	std::string value;
	float step = 0.05f;
	float bigStep = 0.1f;
	float maxValue = 1.0f;
	float minValue = 0.0f;

	ValueSpinBtn* valueSpinBtnUp;
	ValueSpinBtn* valueSpinBtnDown;
	CenteredText* valueText;
	ValueHorizontalSpin(int xc, int yc, float* valueRefc) {
		x = xc;
		y = yc;
		valueRef = valueRefc;
		value = std::to_string(static_cast<int>(round(*valueRef * 100)));
		valueText = new CenteredText(x + width / 3, y, width / 3, height, &value);
		valueSpinBtnUp = new ValueSpinBtn(x + 2 * width / 3, y, width / 3, height, true);
		valueSpinBtnDown = new ValueSpinBtn(x, y, width / 3, height, false);
	}
	void draw(Context& ctx) {
		valueSpinBtnUp->draw(ctx);
		valueText->draw(ctx);
		valueSpinBtnDown->draw(ctx);
	}
	void update(int) {
		value = std::to_string(static_cast<int>(round(*valueRef * 100)));
	}
	rectangle_t hitrectangle() {return rectangle_t(x, y, width, height);}
	void click(int x, int y, int b) {
		if (isIn(x, y, valueSpinBtnUp->hitrectangle())) {
			if (b == 0) {
				(*valueRef) += step;
			}
			else {
				(*valueRef) += bigStep;
			}
			if (*valueRef > maxValue) (*valueRef) = maxValue;
		}
		else if (isIn(x, y, valueSpinBtnDown->hitrectangle())) {
			if (b == 0) {
				(*valueRef) -= step;
			}
			else {
				(*valueRef) -= bigStep;
			}
			if (*valueRef < minValue) (*valueRef) = minValue;
		}
	}
};

class Paginator: public Drawable, public Clickable {
public:
	int x, y, width = 160, height = 35;
	size_t* pageVisibleRef;
	size_t* pageCountRef;

	rgb_t pageIncludedColor = rgb_t(COLOR_BG_LIGHTER.r * 2, COLOR_BG_LIGHTER.g * 2, COLOR_BG_LIGHTER.b * 2);

	CenteredText* pageOne;
	CenteredText* pageTwo;
	CenteredText* pageThree;
	CenteredText* pageFour;
	Paginator(int xc, int yc, size_t* pageVisibleRefc, size_t* pageCountRefc) {
		x = xc;
		y = yc;
		pageVisibleRef = pageVisibleRefc;
		pageCountRef = pageCountRefc;
		pageOne = new CenteredText(x, y, height, height, "1");
		pageTwo = new CenteredText(x + height + 5, y, height, height, "2");
		pageThree = new CenteredText(x + (height + 5) * 2, y, height, height, "3");
		pageFour = new CenteredText(x + (height + 5) * 3, y, height, height, "4");
	}
	void draw(Context& ctx) {
		ctx.rectangle(pageOne->x, pageOne->y, pageOne->width, pageOne->height, (*pageCountRef >= 1) ? pageIncludedColor : COLOR_BG_LIGHTER);
		ctx.emptyRectangle(pageOne->x, pageOne->y, pageOne->width, pageOne->height, (*pageVisibleRef == 0) ? SIZE_LINE_BOLD : SIZE_LINE_THIN, COLOR_LINE);
		pageOne->draw(ctx);
		ctx.rectangle(pageTwo->x, pageTwo->y, pageTwo->width, pageTwo->height, (*pageCountRef >= 2) ? pageIncludedColor : COLOR_BG_LIGHTER);
		ctx.emptyRectangle(pageTwo->x, pageTwo->y, pageTwo->width, pageTwo->height, (*pageVisibleRef == 1) ? SIZE_LINE_BOLD : SIZE_LINE_THIN, COLOR_LINE);
		pageTwo->draw(ctx);
		ctx.rectangle(pageThree->x, pageThree->y, pageThree->width, pageThree->height, (*pageCountRef >= 3) ? pageIncludedColor : COLOR_BG_LIGHTER);
		ctx.emptyRectangle(pageThree->x, pageThree->y, pageThree->width, pageThree->height, (*pageVisibleRef == 2) ? SIZE_LINE_BOLD : SIZE_LINE_THIN, COLOR_LINE);
		pageThree->draw(ctx);
		ctx.rectangle(pageFour->x, pageFour->y, pageFour->width, pageFour->height, (*pageCountRef >= 4) ? pageIncludedColor : COLOR_BG_LIGHTER);
		ctx.emptyRectangle(pageFour->x, pageFour->y, pageFour->width, pageFour->height, (*pageVisibleRef == 3) ? SIZE_LINE_BOLD : SIZE_LINE_THIN, COLOR_LINE);
		pageFour->draw(ctx);
	}
	rectangle_t hitrectangle() {return rectangle_t(x, y, width, height);}
	void click(int x, int y, int) {
		if (isIn(x, y, pageOne->x, pageOne->y, pageOne->width, pageOne->height)) {
			*pageVisibleRef = 0;
		}
		else if (isIn(x, y, pageTwo->x, pageTwo->y, pageTwo->width, pageTwo->height)) {
			*pageVisibleRef = 1;
		}
		else if (isIn(x, y, pageThree->x, pageThree->y, pageThree->width, pageThree->height)) {
			*pageVisibleRef = 2;
		}
		else if (isIn(x, y, pageFour->x, pageFour->y, pageFour->width, pageFour->height)) {
			*pageVisibleRef = 3;
		}
	}
};

class ButtonGrid: public Drawable, public Updatable, public Clickable {
public:
	int x, y;
	int countX, countY;
	HeadCol* h;
	SoundControl* sc;
	size_t pageActive = 0;
	size_t pageVisible = 0;
	size_t pageCount = 1;
	std::vector<std::vector<ButtonCol>> pages;
	std::vector<ValueHorizontalSpin*> controls;
	int active = -1;
	bool running = false;
	int passedTime = 0;
	int bpm = 120;
	ButtonGrid(SoundControl* scc, int xc, int yc, int countXc, std::vector<std::string>& captions) {
		sc = scc;
		x = xc;
		y = yc;
		countX = countXc;
		countY = captions.size();
		h = new HeadCol(scc, x, y, captions);
		pages.push_back(std::vector<ButtonCol>());
		pages.push_back(std::vector<ButtonCol>());
		pages.push_back(std::vector<ButtonCol>());
		pages.push_back(std::vector<ButtonCol>());
		for (int i = 0; i < countX; i++) {
			pages[0].push_back(ButtonCol(x + h->bGap + h->bWidth + i * 35, y, countY, (i % 4 == 0) ? COLOR_BG_LIGHTER : COLOR_BG));
			for (int j = 0; j < countY; j++) {
				controls.push_back(new ValueHorizontalSpin(x + h->bGap + h->bWidth + countX * 35, y + j * 35, &(scc->sampleTracks[j].config->volume)));
			}
		}
		pages[1] = pages[0];
		pages[2] = pages[0];
		pages[3] = pages[0];
	}
	void draw(Context& ctx) {
		h->draw(ctx);
		for (int i = 0; i < countX; i++) {
			pages[pageVisible].at(i).draw(ctx);
		}
		for (int i = 0; i < countY; i++) {
			controls.at(i)->draw(ctx);
		}
	}
	void update(int delta) {
		if (running) {
			passedTime += delta;
			if (passedTime >= 60000 / bpm / 4) {
				passedTime -= 60000 / bpm / 4;
				activateNext();
			}
		}
		for (int i = 0; i < countY; i++) {
			controls.at(i)->update(delta);
		}
	}
	rectangle_t hitrectangle() {return rectangle_t(0, 0, WIN_W, WIN_H);}
	void click(int x, int y, int button) {
		if (isIn(x, y, h->hitrectangle())) {
			h->click(x, y, 0);
		}
		for (int j = 0; j < countX; j++) {
			if (isIn(x, y, pages[pageVisible].at(j).hitrectangle())) {
				pages[pageVisible].at(j).click(x, y, 0);
			}
		}
		for (int i = 0; i < countY; i++) {
			if (isIn(x, y, controls.at(i)->hitrectangle())) {
				controls.at(i)->click(x, y, button);
			}
		}
		pageCount = (!pageIsEmpty(3)) ? 4 : (!pageIsEmpty(2)) ? 3 : (!pageIsEmpty(1)) ? 2 : 1;
	}
	void setVisible(int index) {
		pageVisible = index;
	}
	void activateNext() {
		if (active == countX - 1) {
			if (pageActive >= pageCount) {
				activate(0, 0);
			}
			else {
				activate((pageActive + 1) % pageCount, 0);
			}
			pageVisible = pageActive;
		} else {
			activate(pageActive, active + 1);
		}
	}
	void activate(int page, int index) {
		if (active != -1) {
			pages[pageActive].at(active).dark();
		}
		pageActive = page;
		active = index;
		pages[pageActive].at(active).light();
		for (size_t i = 0; i < pages[pageActive].at(active).b.size(); i++) {
			if (pages[pageActive].at(active).b.at(i).highlited) {
				sc->playSample(i);
			}
		}
	}
	void deactivate() {
		if (active != -1) {
			pages[pageActive].at(active).dark();
			pageActive = 0;
			active = -1;
		}
	}
	void stop() {
		deactivate();
		sc->forceStop();
		active = -1;
		running = false;
	};
	void run() {
		running = true;
		passedTime = 60000 / bpm / 4;
		pageVisible = 0;
	};
	bool pageIsEmpty(int index) {
		for (int i = 0; i < countX; i++) {
			if (!pages[index][i].isEmpty()) return false;;
		}
		return true;
	}
	~ButtonGrid() {
		delete h;
	}
};

class PlayButton: public Drawable, public Clickable {
public:
	int x, y, width;
	rgb_t color;
	bool on = false;
	ButtonGrid* bg;
	PlayButton(int xc, int yc, ButtonGrid* bgc/*, int widthc, int heightc, rgb_t colorc*/) {
		x = xc;
		y = yc;
		width = 55;
		color = COLOR_LINE;
		bg = bgc;
	}
	void draw(Context& ctx) {
		ctx.rectangle(x, y, width, width, COLOR_BG);
		ctx.emptyRectangle(x, y, width, width, SIZE_LINE_BOLD, color);
		int iconWidth = width / 3;
		int leftTopX = x + width / 2 - (iconWidth / 2);
		int leftTopY = y + width / 2 - (iconWidth / 2);
		if (on) {
			ctx.emptyRectangle(leftTopX, leftTopY, iconWidth, iconWidth, COLOR_LINE);
		}
		else {
			ctx.triangle(leftTopX, leftTopY, leftTopX, leftTopY + iconWidth, leftTopX + iconWidth, leftTopY + iconWidth / 2, COLOR_LINE);
		}
	}
	rectangle_t hitrectangle() {
		return rectangle_t(x, y, width, width);
	}
	void togglePlay() {
		on = !on;
		if (!on) {
			bg->stop();
		}
		else {
			bg->run();
		}
	}
	void click(int, int, int) {
		togglePlay();
	}
};

class ControlPanel: public Drawable, public Updatable, public Clickable {
public:
	int x, y, width, height;
	rgb_t color;
	PlayButton* play;
	ValueSpin* valueSpin;
	Paginator* paginator;
	ControlPanel(int xc, int yc, int widthc, int heightc, ButtonGrid* bg) {
		x = xc;
		y = yc;
		width = widthc;
		height = heightc;
		play = new PlayButton(x + 20, y + 20, bg);
		valueSpin = new ValueSpin(x + 200, y + 10, &(bg->bpm));
		paginator = new Paginator(x + 290, y + 35, &(bg->pageVisible), &(bg->pageCount));
	}
	void draw(Context& ctx) {
		ctx.emptyRectangle(x, y, width, height, SIZE_LINE_THIN, COLOR_LINE);
		ctx.draw(*play);
		ctx.draw(*valueSpin);
		ctx.draw(*paginator);
	}
	void update(int delta) {
		valueSpin->update(delta);
	}
	rectangle_t hitrectangle() {return rectangle_t(x, y, width, height);}
	void click(int x, int y, int b) {
		if (isIn(x, y, play->hitrectangle())) {
			play->click(x, y, 0);
		}
		if (isIn(x, y, valueSpin->hitrectangle())) {
			valueSpin->click(x, y, b);
		}
		if (isIn(x, y, paginator->hitrectangle())) {
			paginator->click(x, y, 0);
		}
	}
};

class App: public SDLDevice {
public:
	Context ctx;
	video_buffer_t data;
	int bpm = 120;

	std::map<char, std::function<void()>> keyFunctionMap;

	App(SoundControl* scc, int width, int height, std::vector<std::string> captions):
		SDLDevice(width, height, "Application"),
		data(rectangle_t(0, 0, width, height), COLOR_BG),
		ctx(dynamic_cast<SDLDevice&>(*this), data) {

		ButtonGrid bg(scc, 25, 75, 16, captions);

		ControlPanel cp(0, 500, WIN_W, 100, &bg);

		add(&bg);
		add(&cp);

		addKeyHandler(keys::key_space, [cp]() {cp.play->togglePlay();});
		addKeyHandler('=', [scc]() {scc->masterVolume += 0.05f; if (scc->masterVolume > 100.0f) scc->masterVolume = 100.0f;});
		addKeyHandler('-', [scc]() {scc->masterVolume -= 0.05f; if (scc->masterVolume < 0.0f) scc->masterVolume = 0.0f;});
		addKeyHandler('1', [&bg]() {bg.setVisible(0);});
		addKeyHandler('2', [&bg]() {bg.setVisible(1);});
		addKeyHandler('3', [&bg]() {bg.setVisible(2);});
		addKeyHandler('4', [&bg]() {bg.setVisible(3);});

		launch();
	}
	void launch() {
		start();
		while (ctx.render()) {
			update(updatable);
			render(drawable);
#ifdef SYSTEM_LINUX
			//usleep(60000/bpm/4 * 1000);
#endif
		}
	}
	template <typename T>
	void add(T* object) {
		if (auto ptr = dynamic_cast<Drawable*>(object)) {drawable.push_back(ptr);}
		if (auto ptr = dynamic_cast<Updatable*>(object)) {updatable.push_back(ptr);}
		if (auto ptr = dynamic_cast<Clickable*>(object)) {clickable.push_back(ptr);}
	}
	void addKeyHandler(char key, std::function<void()> lambda) {
		keyFunctionMap[key] = lambda;
	}
	~App() {
		stop();
	}
private:
	int time = getTime();

	std::vector<Drawable*> drawable;
	std::vector<Updatable*> updatable;
	std::vector<Clickable*> clickable;

	int getTime() {
		timeb tb;
		ftime(&tb);
		int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
		return nCount;
	}
	void update(std::vector<Updatable*>& updatable) {
		auto timeAfter = getTime();
		int delta = timeAfter - time;
		time = 2 * getTime() - timeAfter;
		/*if ((timeAfter - time) > 60000/bpm/4) {
			//printf("%d\n", timeAfter);
			time = 2 * getTime() - timeAfter;*/
		for (size_t i = 0; i < updatable.size(); i++) {
			updatable.at(i)->update(delta);
		}
		//}
	}
	void render(std::vector<Drawable*>& drawable) {
		ctx.clear(COLOR_BG);
		for (size_t i = 0; i < drawable.size(); i++) {
			ctx.draw(*(drawable.at(i)));
		}
	}
	/**
	 * Overloaded method for processing mouse buttons.
	 * @param button Index of button that triggered the event
	 * @param pressed true if button was pressed, false if released
	 * @return true, unles some fatal error has occured
	 */
	virtual bool do_mouse_button(const int button, const bool pressed, const int x, const int y)
	{
		if (pressed) {
			for (size_t i = 0; i < clickable.size(); i++) {
				Clickable* c = clickable.at(i);
				if (isIn(x, y, c->hitrectangle())) {
					c->click(x, y, button);
				}
			}
		}
		return true;
	}
	/**
	 * Overloaded method for handling keys from SDL window
	 * @param key  Number of the key pressed, defined in keys.h
	 * @param pressed True if the key was pressed, false if the key was released
	 * @return false if the program should end, true otherwise
	 */
	bool do_key_pressed(const int key, bool pressed) {
		if (pressed) {
			switch (key) {
				case keys::key_escape: return false;
			}
			std::map<char, std::function<void()>>::iterator it = keyFunctionMap.begin();
			while (it != keyFunctionMap.end()) {
				if (key == it->first) {
					it->second();
				}
				it++;
			}
		}
		return true;
	}
};

std::vector<std::string> listFiles(const std::string path) {
	std::vector<std::string> files;
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(path.c_str())) != NULL) {
		printf ("Directory \"%s\" contains next files: ", path.c_str());
		while ((ent = readdir (dir)) != NULL) {
			std::string name = ent->d_name;
			if (name == "." || name == "..") continue;
			printf ("\"%s\", ", name.c_str());
			files.push_back(name);
		}
		printf ("\n");
	} else {
		printf ("Could not open directory \"%s\"\n", path.c_str());
	}
	closedir (dir);
	return files;
}

bool exists(const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

bool isDir(const std::string pathname) {
	struct stat info;
	if( !exists(pathname) ) {
		printf ("Can not access \"%s\"\n", pathname.c_str());
		return false;
	}	
	else if( info.st_mode & S_IFDIR )
		return true;
	else
	    return false;
}

int main(int argc, char **argv) {
	try {
		std::vector<std::string> files;

		if (argc > 1) {
			for (int counter = 1; counter < argc; counter++) {
				std::string file = argv[counter];
				if (!isDir(file)) {
					if (exists(file)) {
						files.push_back(file);
					}
					else {
						printf("File \"%s\" does not exist. Skipping...\n", file.c_str());
					}
				}
				else {
					printf("\"%s\" is a directory. Reading files...\n", file.c_str());
				    for (auto &s : listFiles(file)) {
						if (!isDir(file + s)) {
							files.push_back(file + s);
							printf("File \"%s\" was queued\n", s.c_str());
						}
						else {
							printf("\"%s\" is a directory. Skipping...\n", s.c_str());
						}
			        }
				}
			}
		}
		else {
			files.push_back("/home/dsv/Downloads/mm1/iimavlib-master/data/drum0.wav");
			files.push_back("/home/dsv/Downloads/mm1/iimavlib-master/data/drum1.wav");
			files.push_back("/home/dsv/Downloads/mm1/iimavlib-master/data/drum2.wav");
		}

		std::vector<std::string> captions;
		for (size_t i = 0; i < files.size(); i++) {
			std::string path = files[i];
			auto slash = path.find_last_of("\\/");
			auto dot = path.find_last_of("\\.");
			captions.push_back(path.substr(slash + 1, dot - slash - 1).c_str());
		};

		std::thread t1([files]() {
			audio_id_t device_id = static_cast<audio_id_t>("hw:0,0"); //iimavlib::PlatformDevice::default_device();
			audio_params_t params;
			params.rate = sampling_rate_t::rate_44kHz;
			auto sink = filter_chain<SoundControl>(files)
			            .add<PlatformSink>(device_id)
			            .sink();
			sink->run();
		});

		usleep(1000000); // wait till constructor finish
		std::thread t2([G_SC, captions]() {
			App app(G_SC, 1024, 768, captions);
		});

		t1.detach();
		printf("\nT1 FINISHED\n");
		t2.join();
		printf("\nT2 FINISHED\n");
	}
	catch (std::exception& e) {
		printf("Can not start application\n");
	}
}
