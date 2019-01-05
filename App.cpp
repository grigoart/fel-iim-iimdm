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

/*Preloaded sample data of one metronome tick*/
audio_sample_t metronome_data[] = {20041, 21321, 14, 24908, 14389, 11827, 50, 24948, 0, 2045, 14985, 21855, 15094, 18, -14940, -21055, -14703, -35, 14530, 20392, 14257, 45, -14137, -19720, -13885, 4, 13657, 19127, 13458, -23, -13216, -18554, -13053, 21, 12801, 17996, 12607, 4, -12393, -17424, -12194, -21, 12021, 16863, 11803, 20, -11654, -16302, -11452, -9, 11265, 15771, 11084, -11, -10883, -15257, -10715, 15, 10509, 14768, 10354, -8, -10153, -14282, -9968, -16, 9829, 13779, 9630, 24, -9515, -13263, -9346, 12, 9146, 12829, 9008, -14, -8817, -12390, -8676, -1, 8513, 11944, 8357, 2, -8210, -11513, -8055, -9, 7915, 11083, 7766, 5, -7624, -10666, -7480, -1, 7335, 10265, 7204, -2, -7052, -9883, -6922, 8, 6780, 9504, 6648, 0, -6519, -9138, -6388, -8, 6268, 8773, 6136, 5, -6020, -8415, -5896, -1, 5777, 8075, 5661, -3, -5531, -7747, -5421, 5, 5299, 7429, 5191, 2, -5079, -7112, -4968, -3, 4867, 6806, 4758, 4, -4661, -6509, -4554, 0, 4455, 6220, 4355, -6, -4252, -5943, -4158, 5, 4054, 5674, 3960, -3, -3868, -5415, -3777, -3, 3691, 5158, 3600, 3, -3520, -4908, -3432, 0, 3351, 4671, 3268, -3, -3181, -4442, -3104, 4, 3019, 4225, 2947, 0, -2867, -4011, -2794, -4, 2723, 3800, 2648, 4, -2582, -3595, -2510, 0, 2444, 3403, 2378, -4, -2310, -3219, -2245, 2, 2178, 3043, 2118, -2, -2057, -2870, -1996, -2, 1941, 2703, 1880, 2, -1830, -2543, -1771, 1, 1721, 2390, 1667, -3, -1613, -2244, -1564, 3, 1511, 2108, 1465, -2, -1416, -1974, -1368, -1, 1324, 1843, 1278, 1, -1239, -1718, -1194, 0, 1154, 1600, 1114, -1, -1073, -1490, -1036, 2, 996, 1386, 961, 0, -924, -1285, -888, -1, 856, 1187, 820, 1, -791, -1094, -758, -1, 729, 1006, 698, -2, -669, -925, -641, 2, 613, 848, 585, -1, -561, -778, -536, -1, 512, 708, 488, 1, -467, -643, -444, 0, 423, 582, 403, 1, -381, -526, -363, 1, 343, 473, 327, 0, -309, -424, -292, 0, 276, 379, 261, 0, -245, -336, -231, 1, 217, 297, 204, 0, -191, -263, -179, 0, 168, 231, 157, 1, -147, -200, -136, 0, 127, 173, 118, 0, -110, -149, -101, 1, 93, 126, 86, 0, -79, -106, -71, 0, 66, 89, 60, 0, -55, -74, -49, 0, 45, 60, 40, 0, -36, -48, -32, 0, 28, 37, 25, 0, -22, -29, -19, 0, 17, 22, 15, 0, -12, -16, -11, -1, 8, 11, 8, 1, -5, -7, -4, 0, 4, 5, 3, 1, -2, -2, -2, 0, 1, 2, 1, 0, -1, -1, -1, 0, 0, 0, 0, 0, -1, 0, 0, 0};

/*Each array represents 3x5 image of single letter. 1 = pixel, 0 = empty*/
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
int SLASH[]	=		{0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0};

/*Window width*/
int WIN_W = 865;
/*Window height*/
int WIN_H = 768;

/*Line properties*/
int SIZE_LINE_BOLDEST = 5;
int SIZE_LINE_BOLD = 3;
int SIZE_LINE_THIN = 1;

/*Color properties*/
rgb_t COLOR_BG = rgb_t(25, 25, 25);
rgb_t COLOR_BG_LIGHTER = rgb_t(45, 45, 45);
rgb_t COLOR_LINE = rgb_t(255, 0, 0);
rgb_t COLOR_LINE_INACTIVE = rgb_t(150, 0, 0);

/*Default volume for single track*/
float DEFAULT_VOLUME = 0.5f;

SoundControl* G_SC = nullptr;

/*Check if (x, y) belongs to rectangle(r_x, r_y, r_width, r_height)*/
bool isIn(int x, int y, int r_x, int r_y, int r_width, int r_height) {
	return x > r_x && x < r_x + r_width && y > r_y && y < r_y + r_height;
}
bool isIn(int x, int y, rectangle_t rect) {
	return isIn(x, y, rect.x, rect.y, rect.width, rect.height);
}

/*Drawable virtual class*/
class Drawable {
public:
	virtual void draw(Context& ctx) = 0;
};

/*Updatable virtual class*/
class Updatable {
public:
	virtual void update(int delta) = 0;
};

/*Clickable virtual class*/
class Clickable {
public:
	/*Returns rectangle, representing click area of clickable object*/
	virtual rectangle_t hitrectangle() = 0;
	/*Execute click action. b can be 0, 1 or 3 (left, right, middle mouse button)*/
	virtual void click(int x, int y, int b) = 0;
};

/*Main context*/
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
	/*Render prepared data*/
	int render() {
		return sdl->blit(*data);
	}
	/*Fill with color*/
	void clear(rgb_t color) {data->clear(color);}
	/*Put pixel with color*/
	void pixel(int x, int y, rgb_t color) {(*data)(x, y) = color;}
	/*Draw colored line*/
	void line(int x0, int y0, int x1, int y1, rgb_t color) {draw_line(*data, rectangle_t(x0, y0), rectangle_t(x1, y1), color);}
	/*Draw circle with given radius*/
	void circle(int x, int y, int r, rgb_t color) {circle(x, y, 2 * r, 2 * r, color);}
	/*Draw circle inside given rectangle*/
	void circle(int x, int y, int width, int height, rgb_t color) {draw_circle(*data, rectangle_t(x, y, x + width, y + height), color);}
	/*Draw filled rectangle*/
	void rectangle(int x, int y, int width, int height, rgb_t color) {draw_rectangle(*data, rectangle_t(x, y, width, height), color);}
	/*Draw empty rectangle with border size*/
	void emptyRectangle(int x, int y, int width, int height, int border, rgb_t color) {draw_empty_rectangle(*data, rectangle_t(x, y, width, height), border, color);}
	/*Draw empty rectangle with default border size*/
	void emptyRectangle(int x, int y, int width, int height, rgb_t color) {emptyRectangle(x, y, width, height, 1, color);}
	/*Draw polyline*/
	void polyline(const std::vector<rectangle_t>& points, rgb_t color) {draw_polyline(*data, points, color);}
	/*Draw polygon*/
	void polygon(const std::vector<rectangle_t>& points, rgb_t color) {draw_polyline(*data, points, color);}
	/*Draw triangle*/
	void triangle(int x1, int y1, int x2, int y2, int x3, int y3, rgb_t color) {
		std::vector<rectangle_t> v;
		v.push_back(rectangle_t(x1, y1, 0, 0));
		v.push_back(rectangle_t(x2, y2, 0, 0));
		v.push_back(rectangle_t(x3, y3, 0, 0));
		v.push_back(rectangle_t(x1, y1, 0, 0));
		polygon(v, color);
	};
};

/*Sample configuration*/
class SampleConfig {
public:
	float volume;
	bool muted;
	bool reversed;

	SampleConfig(): volume(DEFAULT_VOLUME), muted(false), reversed(false) {}

};

/*Represents single audio track with given configuration*/
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
		audio_sample_t sample = (config->reversed) ? sampleData->at(sampleData->size() - 1 - position) : sampleData->at(position);
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

/*Audio filter that serve to control audio in application*/
class SoundControl: public AudioFilter {
public:
	std::vector<SampleTrack> sampleTracks;
	float masterVolume = 0.8f;

	std::vector<audio_sample_t>* m_data;

	SoundControl(std::vector<std::string> filesc): AudioFilter(pAudioFilter()) {
		loadMetronome();
		for (int i = 0; i < filesc.size(); i++) {
			loadFileData(filesc[i]);
		}
		G_SC = this;
	}
	
	/*Load metronome tick sample*/
	std::string loadMetronome() {
		m_data = new std::vector<audio_sample_t>(metronome_data, metronome_data + 410);
		sampleTracks.push_back(SampleTrack(*m_data));
		return "Metronome";
	}

	/*Load given wav file. Max duration is 120 sec, 44100Hz and 16 bit.*/
	bool loadFileData(const std::string filename) {
		try {
			size_t timeDurationRestriction = 120;
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

	/*Add sample with given index to playing queue*/
	void playSample(int index) {
		playingSampleTracks.push_back(sampleTracks[index].shallowClone());
	}

	/*Set configuration volume of sample with given index*/
	void setVolume(int index, float vol) {
		sampleTracks[index].config->volume = vol;
	}

	/*Toggle mute in configuration for sample with given index*/
	void toggleMute(int index) {
		sampleTracks[index].config->muted = !sampleTracks[index].config->muted;
	}

	/*Toggle reverse in configuration for sample with given index*/
	void reverse(int index) {
		sampleTracks[index].config->reversed = !sampleTracks[index].config->reversed;
	}

	/*Clear playing queue and clear audio buffer*/
	void forceStop() {
		std::unique_lock<std::mutex> lock(position_mutex);
		playingSampleTracks.clear();
	}

private:
	std::vector<SampleTrack*> playingSampleTracks;

	std::mutex position_mutex;

	/*Mix two given samples. Returns new sample, which is the result of mixing two given samples*/
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

	/*Change sample amplitude (based on master volume value)*/
	void adjustSampleVolume(audio_sample_t& sample) {
		sample.left *= masterVolume;
		sample.right *= masterVolume;
	}
	
	/*Main function*/
	error_type_t do_process(audio_buffer_t& buffer) {
		std::unique_lock<std::mutex> lock(position_mutex);
		auto data = buffer.data.begin();
		auto dataEnd = buffer.data.end();
		while (data != dataEnd) {
			/*No sample track queued*/
			if (playingSampleTracks.size() == 0) {
				/*Fill buffer with zeroes*/
				std::fill(data, dataEnd, 0);
				return error_type_t::ok;
			} else {
				audio_sample_t sampleToWrite;
				/*Iterate through sample track queue, mix all active samples and write to buffer*/
				for (size_t i = 0; i < playingSampleTracks.size();) {
					sampleToWrite = mixSamples(sampleToWrite, playingSampleTracks[i]->getCurrentSample());
					playingSampleTracks[i]->position++;
					/*Check if sample track is finished*/
					if (playingSampleTracks[i]->finished()) {
						playingSampleTracks.erase(playingSampleTracks.begin() + i);
					} else {
						i++;
					}
				}
				adjustSampleVolume(sampleToWrite);
				/*Write mixed sample to buffer*/
				*data++ = sampleToWrite;
			}
		}
		return error_type_t::ok;
	}
};

/*Class represents text object*/
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
			case '/': arr = SLASH; break;
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

/*Class represents text object with auto-centre feature*/
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

/*Class represents single cell (button) in button grid*/
class Button: public Drawable {
public:
	int x, y, width, height;
	rgb_t fillColor;
	bool highlited = false;
	bool on = false;
	Button(int xc, int yc, int widthc, int heightc, rgb_t colorc, rgb_t fillColorc = COLOR_BG) {
		x = xc;
		y = yc;
		width = widthc;
		height = heightc;
		fillColor = fillColorc;
		on = false;
	}
	void draw(Context& ctx) {
		if (highlited) {
			ctx.rectangle(x, y, width, height, rgb_t(COLOR_LINE.r / 2, COLOR_LINE.g / 2, COLOR_LINE.b / 2));
		} else {
			ctx.rectangle(x, y, width, height, fillColor);
		}
		ctx.emptyRectangle(x, y, width, height, (on) ? SIZE_LINE_BOLD : SIZE_LINE_THIN, COLOR_LINE);
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

/*Class represents control button for single header cell*/
class HeadControlButton: public Drawable, public Clickable {
public:
	int x, y;
	static const int height = 14;
	static const int width = 14;
	bool on = true;
	rgb_t color;
	std::function<void()> action;
	HeadControlButton(int xc, int yc, rgb_t colorc, std::function<void()> actionc) : color(colorc) {
		action = actionc;
		x = xc;
		y = yc;
	}
	void draw(Context& ctx) {
		ctx.rectangle(x, y, width, height, (on) ? rgb_t(color.r * 0.9, color.g * 0.9, color.b * 0.9) : rgb_t(color.r * 0.5, color.g * 0.5, color.b * 0.5));
		ctx.emptyRectangle(x, y, width, height, SIZE_LINE_BOLD, color);
	}
	rectangle_t hitrectangle() {
		return rectangle_t(x, y, width, height);
	}
	void click(int, int, int) {
		on = !on;
		action();
	}
};

/*Class represents simple button with lambda action*/
class SmallButton: public Drawable, public Clickable {
public:
	int x, y;
	static const int height = 25;
	static const int width = 25;
	rgb_t color;
	std::function<void()> action;
	SmallButton(int xc, int yc, rgb_t colorc, std::function<void()> actionc) : color(colorc) {
		action = actionc;
		x = xc;
		y = yc;
	}
	void draw(Context& ctx) {
		ctx.rectangle(x, y, width, height, color);
		ctx.emptyRectangle(x, y, width, height, SIZE_LINE_BOLD, rgb_t(color.r / 2, color.g / 2, color.b / 2));
	}
	rectangle_t hitrectangle() {
		return rectangle_t(x, y, width, height);
	}
	void click(int, int, int) {
		action();
	}
};

/*Class represents single header cell*/
class Head: public Drawable {
public:
	int x, y, width, height;
	bool on = true;
	Text text;
	SoundControl* sc;
	HeadControlButton* muteB;
	HeadControlButton* revB;
	int index;
	Head(SoundControl* scc, int indexc, int xc, int yc, int widthc, int heightc, std::string& caption):
		text(xc + 8, yc + 10, &caption, 15) {
		sc = scc;
		index = indexc;
		x = xc;
		y = yc;
		width = widthc;
		height = heightc;
		muteB = new HeadControlButton(x + width - muteB->width - 8, y + height - muteB->height - 8, rgb_t(0, 255, 0), [scc, indexc](){scc->toggleMute(indexc);});
		revB = new HeadControlButton(x + width - revB->width*2 - 8*2, y + height - revB->height - 8, rgb_t(75, 75, 235), [scc, indexc](){scc->reverse(indexc);});
		revB->on = false;
	}
	void draw(Context& ctx) {
		ctx.emptyRectangle(x, y, width, height, (on) ? SIZE_LINE_BOLD : SIZE_LINE_THIN, COLOR_LINE);
		text.draw(ctx);
		muteB->draw(ctx);
		revB->draw(ctx);
	}
	rectangle_t hitrectangle() {
		return rectangle_t(x, y, width, height);
	}
	void click(int x, int y, int) {
		if (isIn(x, y, muteB->hitrectangle())) {
			muteB->click(x, y, 0);
		}
		if (isIn(x, y, revB->hitrectangle())) {
			revB->click(x, y, 0);
		}
	}
};

/*Class represents single column of button cells*/
class ButtonCol: public Drawable, public Clickable {
public:
	int x, y;
	size_t count;
	int bSize = 30;
	int bGap = 5;
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
	void clear() {
		for (size_t i = 0; i < count; i++) {
			if (b.at(i).on) b.at(i).click(0, 0, 0);
		}
		activeCount = 0;
	}
};

/*Class represents header column (containing head cells)*/
class HeadCol: public Drawable, public Clickable {
public:
	int x, y;
	size_t count;
	int bHeight = 30;
	int bWidth = 175;
	int bGap = 5;
	SoundControl* sc;
	std::vector<Head> b;
	HeadCol(SoundControl* scc, int xc, int yc, std::vector<std::string>& captions) {
		sc = scc;
		x = xc;
		y = yc;
		count = captions.size();
		for (size_t i = 0; i < count; i++) {
			b.push_back(Head(scc, i, x, y + (bHeight + bGap) * i, bWidth, bHeight, captions.at(i)));
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

/*Class represents single button for value spinner (+ and -)*/
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

/*Class represents value spinner*/
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

/*Class represents value spinner for master volume value*/
class MasterVolumeSpin: public Drawable, public Updatable, public Clickable {
public:
	int x, y, width = 75, height = 75;
	rgb_t color;
	float* valueRef;
	std::string value;
	float step = 0.05f;
	float bigStep = 0.1f;
	float maxValue = 1.0f;
	float minValue = 0.0f;

	CenteredText* caption;
	ValueSpinBtn* valueSpinBtnUp;
	ValueSpinBtn* valueSpinBtnDown;
	CenteredText* valueText;
	MasterVolumeSpin(int xc, int yc, float* valueRefc) {
		x = xc;
		y = yc;
		valueRef = valueRefc;
		value = std::to_string(static_cast<int>(round(*valueRef * 100)));
		caption = new CenteredText(x, y, width, height / 4, "Master");
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

/*Class represents horizontal value spinner*/
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

/*Class represents object for pagintaing purpose*/
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

/*Class represents complete button grid*/
class ButtonGrid: public Drawable, public Updatable, public Clickable {
public:
	int x, y;
	int countX, countY;
	HeadCol* h;
	SoundControl* sc;
	/*Index of active page*/
	size_t pageActive = 0;
	/*Index of currently visible (opened) page*/
	size_t pageVisible = 0;
	/*Amount of active pages*/
	size_t pageCount = 1;
	/*Pages by itself*/
	std::vector<std::vector<ButtonCol>> pages;
	/*Horizontal spinners to control single track volume*/
	std::vector<ValueHorizontalSpin*> controls;
	/*Index of currently active button column. -1 means playback is stopped*/
	int active = -1;
	/*True if is playing*/
	bool running = false;
	/*Time value from last update*/
	int passedTime = 0;
	/*BPM value*/
	int bpm = 120;
	ButtonGrid(SoundControl* scc, int xc, int yc, int countXc, std::vector<std::string>& captions) {
		sc = scc;
		x = xc;
		y = yc;
		countX = countXc;
		countY = captions.size();
		/*Initialize header column*/
		h = new HeadCol(scc, x, y, captions);
		/*Prepare vectors representing pages*/
		pages.push_back(std::vector<ButtonCol>());
		pages.push_back(std::vector<ButtonCol>());
		pages.push_back(std::vector<ButtonCol>());
		pages.push_back(std::vector<ButtonCol>());
		/*Fill button grid for single page*/
		for (int i = 0; i < countX; i++) {
			pages[0].push_back(ButtonCol(x + h->bGap + h->bWidth + i * 35, y, countY, (i % 4 == 0) ? COLOR_BG_LIGHTER : COLOR_BG));
			for (int j = 0; j < countY; j++) {
				controls.push_back(new ValueHorizontalSpin(x + h->bGap + h->bWidth + countX * 35, y + j * 35, &(scc->sampleTracks[j].config->volume)));
			}
		}
		/*Initialize rest of the pages*/
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
			/*Count delta time*/
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
		recalcPages();
	}
	/*Recalculate number of active pages*/
	void recalcPages() {
		pageCount = (!pageIsEmpty(3)) ? 4 : (!pageIsEmpty(2)) ? 3 : (!pageIsEmpty(1)) ? 2 : 1;
	}
	/*Make page with specific index visible*/
	void setVisible(int index) {
		pageVisible = index;
	}
	/*Activate next column/page*/
	void activateNext() {
		/*This is the last button column on the page*/
		if (active == countX - 1) {
			/*Current page is last active*/
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
	/*Activate button column with specific symbol on the page with specific symbol*/
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
	/*Deactivate current column*/
	void deactivate() {
		if (active != -1) {
			pages[pageActive].at(active).dark();
			pageActive = 0;
			active = -1;
		}
	}
	/*Stop running grid*/
	void stop() {
		deactivate();
		sc->forceStop();
		active = -1;
		running = false;
	};
	/*Start running grid*/
	void run() {
		running = true;
		passedTime = 60000 / bpm / 4;
		pageVisible = 0;
	};
	/*Check if page contains any active button cell*/
	bool pageIsEmpty(int index) {
		for (int i = 0; i < countX; i++) {
			if (!pages[index][i].isEmpty()) return false;;
		}
		return true;
	}
	/*Deactivate all button cells on current visible page*/
	void clear() {
		for (int i = 0; i < countX; i++) {
			pages[pageVisible][i].clear();
		}
		recalcPages();
	}
	void loadPreset(int preset) {
		if (preset == 1) {

		}
		else if (preset == 2) {

		}
		else if (preset == 3) {

		}
	}
	~ButtonGrid() {
		delete h;
	}
};

class PlayButton: public Drawable, public Clickable {
public:
	int x, y, width;
	bool on = false;
	ButtonGrid* bg;
	PlayButton(int xc, int yc, ButtonGrid* bgc/*, int widthc, int heightc, rgb_t colorc*/) {
		x = xc;
		y = yc;
		width = 55;
		bg = bgc;
	}
	void draw(Context& ctx) {
		ctx.rectangle(x, y, width, width, COLOR_BG);
		ctx.emptyRectangle(x, y, width, width, SIZE_LINE_BOLD, COLOR_LINE);
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

	CenteredText* colorText;
	SmallButton* colorRed;
	SmallButton* colorGreen;
	SmallButton* colorBlue;

	Text* hint1;
	Text* hint2;
	Text* hint3;
	Text* hint4;

	PlayButton* play;
	MasterVolumeSpin* masterVolumeSpin;
	ValueSpin* valueSpin;
	CenteredText* text;
	Paginator* paginator;
	ControlPanel(int xc, int yc, int widthc, int heightc, ButtonGrid* bg) {
		x = xc;
		y = yc;
		width = widthc;
		height = heightc;

		colorText = new CenteredText(x + 175 + 100 - 5, y + 10, 30*3, 35, "COLOR SCHEME");
		colorRed = new SmallButton(x + 175 + 100, y + 45, rgb_t(200, 0, 0), [COLOR_LINE, COLOR_LINE_INACTIVE](){COLOR_LINE = rgb_t(225, 25, 25); COLOR_LINE_INACTIVE = rgb_t(155, 0, 0);});
		colorGreen = new SmallButton(x + 205 + 100, y + 45, rgb_t(0, 200, 0), [COLOR_LINE, COLOR_LINE_INACTIVE](){COLOR_LINE = rgb_t(25, 200, 25); COLOR_LINE_INACTIVE = rgb_t(0, 155, 0);});
		colorBlue = new SmallButton(x + 235 + 100, y + 45, rgb_t(0, 0, 200), [COLOR_LINE, COLOR_LINE_INACTIVE](){COLOR_LINE = rgb_t(25, 25, 225); COLOR_LINE_INACTIVE = rgb_t(0, 0, 155);});

		hint1 = new Text(x + 115, y + 15, "1234  - PAGES");
		hint2 = new Text(x + 115, y + 35, "SPACE - STOP/PLAY");
		hint3 = new Text(x + 115, y + 55, "C     - CLEAR");
		hint4 = new Text(x + 115, y + 75, "ESC   - EXIT");

		play = new PlayButton(x + 25, y + 20, bg);
		masterVolumeSpin = new MasterVolumeSpin(x + width - 25*2 - 100*2, y + 10, &(bg->sc->masterVolume));
		valueSpin = new ValueSpin(x + width - 25 - 100, y + 10, &(bg->bpm));
		text = new CenteredText(x + width - 25*4 - 85*2 - 40*4, y + 10, 40*4, 35, "PAGE");
		paginator = new Paginator(x + width - 25*4 - 85*2 - 40*4, y + 40, &(bg->pageVisible), &(bg->pageCount));
	}
	void draw(Context& ctx) {
		ctx.emptyRectangle(x, y, width, height, SIZE_LINE_THIN, COLOR_LINE);

		ctx.draw(*colorText);
		ctx.draw(*colorRed);
		ctx.draw(*colorGreen);
		ctx.draw(*colorBlue);

		ctx.draw(*hint1);
		ctx.draw(*hint2);
		ctx.draw(*hint3);
		ctx.draw(*hint4);

		ctx.draw(*play);
		ctx.draw(*masterVolumeSpin);
		ctx.draw(*valueSpin);
		ctx.draw(*paginator);
		ctx.draw(*text);
	}
	void update(int delta) {
		masterVolumeSpin->update(delta);
		valueSpin->update(delta);
	}
	rectangle_t hitrectangle() {return rectangle_t(x, y, width, height);}
	void click(int x, int y, int b) {
		if (isIn(x, y, play->hitrectangle())) {
			play->click(x, y, 0);
		}

		if (isIn(x, y, colorRed->hitrectangle())) {
			colorRed->click(x, y, b);
		}
		if (isIn(x, y, colorGreen->hitrectangle())) {
			colorGreen->click(x, y, b);
		}
		if (isIn(x, y, colorBlue->hitrectangle())) {
			colorBlue->click(x, y, b);
		}

		if (isIn(x, y, masterVolumeSpin->hitrectangle())) {
			masterVolumeSpin->click(x, y, b);
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

		ButtonGrid bg(scc, 25, 25, 16, captions);

		ControlPanel cp(25, WIN_H - 25 - 100, WIN_W - 50, 100, &bg);

		add(&bg);
		add(&cp);

		addKeyHandler(keys::key_space, [cp]() {cp.play->togglePlay();});
		addKeyHandler('1', [&bg]() {bg.setVisible(0);});
		addKeyHandler('2', [&bg]() {bg.setVisible(1);});
		addKeyHandler('3', [&bg]() {bg.setVisible(2);});
		addKeyHandler('4', [&bg]() {bg.setVisible(3);});
		addKeyHandler('c', [&bg]() {bg.clear();});
		addKeyHandler('q', [&bg]() {bg.loadPreset(1);});
		addKeyHandler('w', [&bg]() {bg.loadPreset(2);});
		addKeyHandler('e', [&bg]() {bg.loadPreset(3);});

		launch();
	}
	void launch() {
		start();
		while (ctx.render()) {
			update(updatable);
			render(drawable);
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
		time = timeAfter;
		for (size_t i = 0; i < updatable.size(); i++) {
			updatable.at(i)->update(delta);
		}
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
		}

		std::vector<std::string> captions;
		captions.push_back("Metronome");
		for (size_t i = 0; i < files.size(); i++) {
			std::string path = files[i];
			auto slash = path.find_last_of("\\/");
			auto dot = path.find_last_of("\\.");
			captions.push_back(path.substr(slash + 1, dot - slash - 1).c_str());
		};

		WIN_H = 100 + 25*3 + 35 * captions.size();

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
			App app(G_SC, WIN_W, WIN_H, captions);
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
